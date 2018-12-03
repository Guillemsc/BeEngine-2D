#include "GameObjectAbstractor.h"
#include "ModuleJson.h"
#include "GameObject.h"
#include "GameObjectComponent.h"
#include "ModuleGameObject.h"
#include "App.h"
#include "ComponentTransfrom.h"

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

void GameObjectAbstraction::Serialize(const std::string & path)
{
}

void GameObjectAbstraction::DeSerialize(const std::string & path)
{
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
