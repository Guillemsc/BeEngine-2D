#include "GameObjectAbstraction.h"
#include "ModuleJson.h"
#include "GameObject.h"
#include "GameObjectComponent.h"
#include "ModuleGameObject.h"
#include "App.h"
#include "ComponentTransfrom.h"
#include "ModuleFileSystem.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

GameObjectAbstraction::GameObjectAbstraction()
{
}

GameObjectAbstraction::~GameObjectAbstraction()
{
}

void GameObjectAbstraction::Abstract(const std::vector<GameObject*>& to_abstract)
{
	Clear();

	uint id_counter = 0;

	std::vector<GameObject*> all_to_abstract;

	for (std::vector<GameObject*>::const_iterator it = to_abstract.cbegin(); it != to_abstract.cend(); ++it)
		all_to_abstract.push_back((*it));

	std::map<uint, GameObject*> temp_relations;

	while (all_to_abstract.size() > 0)
	{
		GameObject* curr_go = *all_to_abstract.begin();

		temp_relations[id_counter++] = curr_go;

		std::vector<GameObject*> childs = curr_go->GetChilds();

		if(curr_go->GetChildsCount() > 0)
			all_to_abstract.insert(all_to_abstract.end(), childs.begin(), childs.end());

		all_to_abstract.erase(all_to_abstract.begin());
	}

	for (std::map<uint, GameObject*>::iterator it = temp_relations.begin(); it != temp_relations.end(); ++it)
	{
		GameObject* curr_go = (*it).second;

		int parent_id = -1;

		GameObject* parent_go = curr_go->GetParent();

		if (parent_go != nullptr)
		{
			for (std::map<uint, GameObject*>::iterator ch = temp_relations.begin(); ch != temp_relations.end(); ++ch)
			{
				if ((*ch).second == parent_go)
				{
					parent_id = (*ch).first;
					break;
				}
			}
		}

		std::vector<GameObjectComponent*> components = curr_go->GetComponents();

		GameObjectAbstractionRelation curr_relation;
		curr_go->OnSaveAbstraction(curr_relation.go_abstraction);

		curr_relation.go_abstraction.AddInt("components_count", components.size());

		for (std::vector<GameObjectComponent*>::iterator it = components.begin(); it != components.end(); ++it)
		{
			GameObjectComponent* comp = (*it);

			DataAbstraction components_abstraction;

			components_abstraction.AddInt("type", comp->GetType());

			comp->OnSaveAbstraction(components_abstraction);

			curr_relation.components_abstraction.push_back(components_abstraction);
		}

		curr_relation.id = (*it).first;
		curr_relation.parent_id = parent_id;

		relations.push_back(curr_relation);
	}

	abstracted = true;
}

std::vector<GameObject*> GameObjectAbstraction::DeAbstract()
{
	std::vector<GameObject*> ret;

	if (abstracted)
	{
		class TempRelationStruct
		{
		public:
			GameObjectAbstractionRelation relation;
			GameObject* go = nullptr;
		};

		std::vector<TempRelationStruct> temp_relations;

		for (std::vector<GameObjectAbstractionRelation>::iterator it = relations.begin(); it != relations.end(); ++it)
		{
			GameObject* go = App->gameobject->CreateGameObject();
			ret.push_back(go);

			go->OnLoadAbstraction((*it).go_abstraction);

			std::vector<DataAbstraction> components_abstractions = (*it).components_abstraction;

			for (std::vector<DataAbstraction>::iterator c = components_abstractions.begin(); c != components_abstractions.end(); ++c)
			{
				int type = (*c).GetInt("type", -1);

				if (type > -1)
				{
					if (type == 0)
					{
						go->transform->OnLoadAbstraction((*c));
					}
					else
					{
						GameObjectComponent* new_component = go->CreateComponent(static_cast<ComponentType>(type));
						new_component->OnLoadAbstraction((*c));
					}
				}
			}

			TempRelationStruct str;
			str.relation = (*it);
			str.go = go;

			temp_relations.push_back(str);
		}

		for (std::vector<TempRelationStruct>::iterator it = temp_relations.begin(); it != temp_relations.end(); ++it)
		{
			if ((*it).relation.parent_id > -1)
			{
				GameObject* curr_go = (*it).go;

				for (std::vector<TempRelationStruct>::iterator ch = temp_relations.begin(); ch != temp_relations.end(); ++ch)
				{
					if ((*it).relation.parent_id == (*ch).relation.id)
					{
						curr_go->SetParent((*ch).go);
						break;
					}
				}
			}
		}
	}

	return ret;
}

bool GameObjectAbstraction::Serialize(const std::string& path, const std::string& name, const std::string& extension)
{
	bool ret = false;

	if (abstracted)
	{
		if (App->file_system->FolderExists(path))
		{
			std::string filepath = path + name + "." + extension;

			if(App->file_system->FileExists(filepath))
				App->file_system->FileDelete(filepath);

			JSON_Doc* doc = App->json->CreateJSON(filepath.c_str());

			if (doc != nullptr)
			{
				doc->MoveToRoot();

				doc->SetNumber("game_objects_count", relations.size());

				int go_count = 0;
				for (std::vector<GameObjectAbstractionRelation>::iterator it = relations.begin(); it != relations.end(); ++it)
				{
					doc->MoveToRoot();

					std::string curr_go_section = "game_object_" + std::to_string(go_count++);
					doc->AddSection(curr_go_section);

					if (doc->MoveToSection(curr_go_section))
					{
						JSON_Doc go_node = doc->GetNode();

						go_node.SetNumber("id", (*it).id);
						go_node.SetNumber("parent_id", (*it).parent_id);

						(*it).go_abstraction.Serialize(go_node);

						doc->SetArray("components");

						int component_count = 0;

						for (std::vector<DataAbstraction>::iterator c = (*it).components_abstraction.begin(); c != (*it).components_abstraction.end(); ++c)
						{
							JSON_Doc comp_node = doc->GetNode();

							comp_node.AddSectionToArray("components");

							if (comp_node.MoveToSectionFromArray("components", component_count++))
							{
								(*c).Serialize(comp_node);
							}
						}
					}
				}

				doc->Save();
				doc->MoveToRoot();

				App->json->UnloadJSON(doc);

				ret = true;
			}
		}
	}

	return ret;
}

bool GameObjectAbstraction::DeSerialize(const std::string & filepath)
{
	bool ret = false;

	Clear();

	if (App->file_system->FileExists(filepath))
	{
		JSON_Doc* doc = App->json->LoadJSON(filepath.c_str());

		if (doc != nullptr)
		{
			int game_objects_count = doc->GetNumber("game_objects_count", 0);

			for (int i = 0; i < game_objects_count; ++i)
			{
				doc->MoveToRoot();

				std::string curr_go_section = "game_object_" + std::to_string(i);

				if (doc->MoveToSection(curr_go_section))
				{
					JSON_Doc go_node = doc->GetNode();

					GameObjectAbstractionRelation curr_relation;

					curr_relation.id = go_node.GetNumber("id", -1);
					curr_relation.parent_id = go_node.GetNumber("parent_id", -1);

					if (curr_relation.id > -1)
					{
						curr_relation.go_abstraction.DeSerialize(go_node);

						int components_count = go_node.GetArrayCount("components");

						for (int c = 0; c < components_count; ++c)
						{
							JSON_Doc comp_node = go_node.GetNode();

							if (comp_node.MoveToSectionFromArray("components", c))
							{
								DataAbstraction comp_data;
								comp_data.DeSerialize(comp_node);

								curr_relation.components_abstraction.push_back(comp_data);
							}
						}
					}

					relations.push_back(curr_relation);
				}
			}

			abstracted = true;
			ret = true;

			App->json->UnloadJSON(doc);
		}
	}

	return ret;
}

void GameObjectAbstraction::Clear()
{
	relations.clear();
	abstracted = false;
}

uint GameObjectAbstraction::GetParentId(uint id)
{
	uint ret = -1;


	return ret;
}

GameObjectAbstractionRelation::GameObjectAbstractionRelation()
{
}
