#include "GameObjectAbstractor.h"
#include "ModuleJson.h"
#include "GameObject.h"
#include "GameObjectComponent.h"
#include "ModuleGameObject.h"
#include "App.h"
#include "ComponentTransfrom.h"
#include "ModuleFileSystem.h"

GameObjectAbstraction::GameObjectAbstraction()
{
}

GameObjectAbstraction::~GameObjectAbstraction()
{
}

void GameObjectAbstraction::Clear()
{
	loaded = false;

	go_to_ids_relations.clear();
	child_to_parent_relations.clear();
}

void GameObjectAbstraction::Abstract(GameObject * go)
{
	Clear();

	if (go != nullptr)
	{
		uint curr_id = 0;

		std::vector<GameObject*> to_add;

		to_add.push_back(go);

		while (!to_add.empty())
		{
			GameObject* curr = *to_add.begin();
			
			int go_id = GetIdToGoRelation(curr);

			if (go_id == -1)
			{
				AddGoToIdRelation(curr, curr_id);
				go_id = curr_id;
			}

			if (curr->GetParent() != nullptr)
			{
				int parent_go_id = GetIdToGoRelation(curr->GetParent());

				if (parent_go_id == -1)
				{
					AddGoToIdRelation(curr, curr_id);
					parent_go_id = curr_id;
				}

				AddChildToParentRelation(go_id, parent_go_id);
			}
			
			std::vector<GameObject*> childs = curr->GetChilds();
			for (std::vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
				to_add.push_back(*it);

			to_add.erase(to_add.begin());
		}

		loaded = true;
	}
}

GameObject* GameObjectAbstraction::DeAbstract()
{
	GameObject* ret = nullptr;

	bool first_go = true;

	if (loaded)
	{
		for (std::vector<GoToIdRelation>::iterator it = go_to_ids_relations.begin(); it != go_to_ids_relations.end(); ++it)
		{
			GameObject* obj = App->gameobject->CreateGameObject();

			if (first_go)
			{
				ret = obj;
				first_go = false;
			}

			if (obj != nullptr)
			{
				obj->OnLoadAbstraction((*it).GetDataAbstraction());

				std::vector<DataAbstraction> components_data = (*it).GetComponentsDataAbstraction();
				for (std::vector<DataAbstraction>::iterator comp = components_data.begin(); comp != components_data.end(); ++comp)
				{
					int comp_int = (*comp).GetInt("type", -1);

					if (comp_int != -1)
					{
						ComponentType type = static_cast<ComponentType>(comp_int);

						if (type != ComponentType::COMPONENT_TYPE_TRANSFORM)
						{
							GameObjectComponent* component = obj->CreateComponent(type);

							if (component != nullptr)
								component->OnLoadAbstraction(*comp);
						}
						else
						{
							obj->transform->OnLoadAbstraction(*comp);
						}
					}
				}

				(*it).SetGo(obj);
			}
		}

		for (std::vector<ChildToParentRelation>::iterator it = child_to_parent_relations.begin(); it != child_to_parent_relations.end(); ++it)
		{
			uint id = (*it).GetId();
			uint id_parent = (*it).GetParentId();

			GameObject* obj = nullptr;
			GameObject* parent_obj = nullptr;
			
			obj = GetGoToIdRelation(id);
			parent_obj = GetGoToIdRelation(id_parent);

			if (obj != nullptr && parent_obj != nullptr)
				obj->SetParent(parent_obj);
		}
	}

	return ret;
}

void GameObjectAbstraction::Serialize(const std::string& path, const std::string& name, const std::string& extension)
{
	if (loaded)
	{
		if (App->file_system->FolderExists(path))
		{
			std::string filepath = path + name + "." + "extension";

			if (App->file_system->FileExists(filepath.c_str()))
				App->file_system->FileDelete(filepath.c_str());

			JSON_Doc* doc = App->json->CreateJSON(filepath.c_str());

			if (doc != nullptr)
			{
				int curr_go_count = 0;
				doc->SetNumber("GameObjectsCount", go_to_ids_relations.size());

				for (std::vector<GoToIdRelation>::iterator it = go_to_ids_relations.begin(); it != go_to_ids_relations.end(); ++it, ++curr_go_count)
				{
					doc->MoveToRoot();

					std::string curr_go_section_name = "GameObject_" + std::to_string(curr_go_count);
					doc->AddSection(curr_go_section_name);

					if (doc->MoveToSection(curr_go_section_name))
					{
						JSON_Doc go_node = doc->GetNode();

						go_node.SetNumber("id", (*it).GetId());
						go_node.SetNumber("parent_id", GetParentFromChild((*it).GetId()));

						(*it).GetDataAbstraction().Serialize(go_node);

						doc->SetArray("Components");

						bool curr_comp_count = 0;
						std::vector<DataAbstraction> components_data = (*it).GetComponentsDataAbstraction();
						for (std::vector<DataAbstraction>::iterator comp = components_data.begin(); comp != components_data.end(); ++comp, ++curr_comp_count)
						{
							JSON_Doc comp_node = doc->GetNode();

							comp_node.AddSectionToArray("Components");

							if (comp_node.MoveToSectionFromArray("Components", curr_comp_count))
							{
								(*comp).Serialize(comp_node);
							}
						}
					}
				}

				doc->Save();

				App->json->UnloadJSON(doc);
			}
		}
	}
}

void GameObjectAbstraction::DeSerialize(const std::string & filepath)
{
	Clear();

	if (App->file_system->FileExists(filepath))
	{
		JSON_Doc* doc = App->json->LoadJSON(filepath.c_str());

		if (doc != nullptr)
		{
			int game_objects_count = doc->GetNumber("GameObjectsCount", 0);

			for (int i = 0; i < game_objects_count; ++i)
			{
				doc->MoveToRoot();

				std::string curr_go_section_name = "GameObject_" + std::to_string(i);

				if (doc->MoveToSection(curr_go_section_name))
				{
					JSON_Doc go_node = doc->GetNode();

					uint id = go_node.GetNumber("id", -1);
					int parent_id = go_node.GetNumber("parent_id", -1);

					if (id != -1)
					{
						if (parent_id != -1)
							AddChildToParentRelation(id, parent_id);
						
						DataAbstraction go_abstraction;
						go_abstraction.DeSerialize(go_node);

						std::vector<DataAbstraction> components_abstractions;

						int components_count = go_node.GetArrayCount("Components");

						for (int c = 0; c < components_count; ++c)
						{
							JSON_Doc comp_node = go_node;

							if (comp_node.MoveToSectionFromArray("Components", c))
							{
								DataAbstraction comp_data;
								comp_data.DeSerialize(comp_node);

								components_abstractions.push_back(comp_data);
							}
						}

						AddIdToAbstractionRelation(id, go_abstraction, components_abstractions);
					}
				}
			}

			loaded = true;

			App->json->UnloadJSON(doc);
		}
	}
}

void GameObjectAbstraction::AddGoToIdRelation(GameObject * go, uint & curr_id)
{
	if (go != nullptr)
	{
		++curr_id;

		DataAbstraction abs;
		std::vector<DataAbstraction> components_abs;

		go->OnSaveAbstraction(abs);

		std::vector<GameObjectComponent*> components_list = go->GetComponents();

		for (std::vector<GameObjectComponent*>::iterator it = components_list.begin(); it != components_list.end(); ++it)
		{
			GameObjectComponent* curr = (*it);

			DataAbstraction comp_abs;
			comp_abs.AddInt("type", curr->GetType());
			curr->OnSaveAbstraction(comp_abs);

			components_abs.push_back(comp_abs);
		}

		GoToIdRelation relation(curr_id, go, abs, components_abs);

		go_to_ids_relations.push_back(relation);
	}
}

void GameObjectAbstraction::AddIdToAbstractionRelation(uint id, DataAbstraction go_abs, std::vector<DataAbstraction> components_abs)
{
	GoToIdRelation go_relation(id, go_abs, components_abs);
	go_to_ids_relations.push_back(go_relation);
}

void GameObjectAbstraction::AddChildToParentRelation(uint id, uint parent_id)
{	
	ChildToParentRelation relation(id, parent_id);
	child_to_parent_relations.push_back(relation);
}

int GameObjectAbstraction::GetParentFromChild(int child_id)
{
	int ret = -1;

	for (std::vector<ChildToParentRelation>::iterator it = child_to_parent_relations.begin(); it != child_to_parent_relations.end(); ++it)
	{
		if ((*it).GetId() == child_id)
		{
			ret = (*it).GetParentId();
			break;
		}
	}

	return ret;
}

GameObject * GameObjectAbstraction::GetGoToIdRelation(int id)
{
	GameObject* ret = nullptr;

	for (std::vector<GoToIdRelation>::iterator it = go_to_ids_relations.begin(); it != go_to_ids_relations.end(); ++it)
	{
		if ((*it).GetId() == id)
		{
			ret = (*it).GetGo();
			break;
		}
	}

	return ret;
}

int GameObjectAbstraction::GetIdToGoRelation(GameObject * go)
{
	int ret = -1;

	for (std::vector<GoToIdRelation>::iterator it = go_to_ids_relations.begin(); it != go_to_ids_relations.end(); ++it)
	{
		if ((*it).GetGo() == go)
		{
			ret = (*it).GetId();
			break;
		}
	}

	return ret;
}

GoToIdRelation::GoToIdRelation(uint _id, GameObject * _go, const DataAbstraction& abs, const std::vector<DataAbstraction>& components_abs)
{
	id = _id;
	go = _go;
	data_game_object = abs;
	data_components = components_abs;
}

GoToIdRelation::GoToIdRelation(uint _id, const DataAbstraction & abs, const std::vector<DataAbstraction>& components_abs)
{
	id = _id;
	data_game_object = abs;
	data_components = components_abs;
}

void GoToIdRelation::SetGo(GameObject * _go)
{
	go = _go;
}

uint GoToIdRelation::GetId() const
{
	return id;
}

DataAbstraction GoToIdRelation::GetDataAbstraction() const
{
	return data_game_object;
}

std::vector<DataAbstraction> GoToIdRelation::GetComponentsDataAbstraction() const
{
	return data_components;
}

GameObject * GoToIdRelation::GetGo() const
{
	return go;
}

ChildToParentRelation::ChildToParentRelation(uint _id, uint _id_parent)
{
	id = _id;
	id_parent = _id_parent;
}

uint ChildToParentRelation::GetId() const
{
	return id;
}

uint ChildToParentRelation::GetParentId() const
{
	return id_parent;
}
