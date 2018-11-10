#include "ModuleGameObject.h"
#include "Functions.h"

ModuleGameObject::ModuleGameObject()
{
}

ModuleGameObject::~ModuleGameObject()
{
}

bool ModuleGameObject::Awake()
{
	bool ret = true;


	return ret;
}

bool ModuleGameObject::Start()
{
	bool ret = true;

	return ret;
}

bool ModuleGameObject::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool ModuleGameObject::Update()
{
	bool ret = true;

	return ret;
}

bool ModuleGameObject::PostUpdate()
{
	bool ret = true;

	ActuallyDestroyGameObjects();

	return ret;
}

bool ModuleGameObject::CleanUp()
{
	bool ret = true;

	DestroyAllGameObjects();

	return ret;
}

GameObject* ModuleGameObject::CreateGameObject()
{
	GameObject* ret = nullptr;

	ret = new GameObject(GetUIDRandomHexadecimal());

	std::string name = "GameObject" + std::to_string(game_objects.size());
	ret->SetName(name.c_str());

	game_objects.push_back(ret);

	AddGameObjectToRoot(ret);

	ret->Start();

	return ret;
}

void ModuleGameObject::DestroyGameObject(GameObject * go)
{
	if (go != nullptr)
	{
		std::vector<GameObject*> to_check;
		to_check.push_back(go);

		while (!to_check.empty())
		{
			GameObject* curr = *to_check.begin();

			for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
			{
				if ((*it) == curr)
				{
					game_objects.erase(it);
					break;
				}
			}

			bool found = false;
			for (std::vector<GameObject*>::iterator it = game_objects_to_destroy.begin(); it != game_objects_to_destroy.end(); ++it)
			{
				if ((*it) == curr)
				{
					found = true;
					break;
				}
			}

			if (!found)
			{
				game_objects_to_destroy.push_back(curr);
			}

			if(curr->GetSelected())
				RemoveGameObjectFromSelected(curr);

			to_check.erase(to_check.begin());

			to_check.insert(to_check.begin(), curr->childs.begin(), curr->childs.end());
		}
	}
}

GameObject * ModuleGameObject::GetGameObjectByUID(const char * uid)
{
	GameObject* ret = nullptr;

	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		if ((*it)->GetUID().compare(uid) == 0)
		{
			ret = *it;
			break;
		}
	}

	return ret;
}

std::vector<GameObject*> ModuleGameObject::GetRootGameObjects() const
{
	return game_objects_root;
}

void ModuleGameObject::AddGameObjectToSelected(GameObject * go)
{
	if (go != nullptr)
	{
		bool found = false;
		for (std::vector<GameObject*>::iterator it = game_objects_selected.begin(); it != game_objects_selected.end(); ++it)
		{
			if (go == (*it))
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			go->selected = true;

			game_objects_selected.push_back(go);
		}
	}
}

void ModuleGameObject::RemoveGameObjectFromSelected(GameObject * go)
{
	if (go != nullptr)
	{
		for (std::vector<GameObject*>::iterator it = game_objects_selected.begin(); it != game_objects_selected.end(); ++it)
		{
			if (go == (*it))
			{
				go->selected = false;

				game_objects_selected.erase(it);
				break;
			}
		}
	}
}

void ModuleGameObject::RemoveAllGameObjectsFromSelected()
{
	for (std::vector<GameObject*>::iterator it = game_objects_selected.begin(); it != game_objects_selected.end(); ++it)
	{
		(*it)->selected = false;
	}

	game_objects_selected.clear();
}

std::vector<GameObject*> ModuleGameObject::GetSelectedGameObjects() const
{
	return game_objects_selected;
}

uint ModuleGameObject::GetSelectedGameObjectsCount() const
{
	return game_objects_selected.size();
}

void ModuleGameObject::DestroyAllGameObjects()
{
	std::vector<GameObject*> all_game_objects = game_objects;

	for (std::vector<GameObject*>::iterator it = all_game_objects.begin(); it != all_game_objects.end(); ++it)
	{
		DestroyGameObject(*it);
	}

	ActuallyDestroyGameObjects();
}

void ModuleGameObject::ActuallyDestroyGameObjects()
{
	for (std::vector<GameObject*>::iterator it = game_objects_to_destroy.begin(); it != game_objects_to_destroy.end(); ++it)
	{
		std::vector<GameObject*> childs = (*it)->childs;

		for (std::vector<GameObject*>::iterator ch = childs.begin(); ch != childs.end(); ++ch)
		{
			(*ch)->RemoveParent();
		}

		(*it)->RemoveParent();

		(*it)->CleanUp();
		RELEASE(*it);
	}

	game_objects_to_destroy.clear();
}

void ModuleGameObject::AddGameObjectToRoot(GameObject * go)
{
	if (go != nullptr)
	{
		bool found = false;
		for (std::vector<GameObject*>::iterator it = game_objects_root.begin(); it != game_objects_root.end(); ++it)
		{
			if (go == (*it))
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			game_objects_root.push_back(go);
		}
	}
}

void ModuleGameObject::RemoveGameObjectFromRoot(GameObject * go)
{
	if (go != nullptr)
	{
		for (std::vector<GameObject*>::iterator it = game_objects_root.begin(); it != game_objects_root.end(); ++it)
		{
			if (go == (*it))
			{
				game_objects_root.erase(it);
				break;
			}
		}
	}
}

void ModuleGameObject::ChangeGameObjectPositionOnRootList(GameObject * go, uint new_pos)
{
	if (go != nullptr)
	{
		bool exists = false;
		for (std::vector<GameObject*>::iterator it = game_objects_root.begin(); it != game_objects_root.end(); ++it)
		{
			if ((*it) == go)
			{
				game_objects_root.erase(it);
				break;
			}
		}

		if (new_pos > game_objects_root.size())
			new_pos = game_objects_root.size();

		game_objects_root.insert(game_objects_root.begin() + new_pos, go);
	}
}

void ModuleGameObject::ChangeGameObjectPositionOnParentChildren(GameObject * go, uint new_pos)
{
	if (go != nullptr && go->GetParent() != nullptr)
	{
		GameObject* parent = go->GetParent();

		bool exists = false;
		for (std::vector<GameObject*>::iterator it = parent->childs.begin(); it != parent->childs.end(); ++it)
		{
			if ((*it) == go)
			{
				parent->childs.erase(it);
				break;
			}
		}

		if (new_pos > parent->childs.size())
			new_pos = parent->childs.size();

		parent->childs.insert(parent->childs.begin() + new_pos, go);
	}
}
