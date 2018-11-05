#include "ModuleGameObject.h"

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

	ret = new GameObject();
	ret->Start();

	game_objects.push_back(ret);

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

			to_check.erase(to_check.begin());

			to_check.insert(to_check.begin(), curr->childs.begin(), curr->childs.end());
		}
	}
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
