#include "GameObject.h"
#include "App.h"
#include "ModuleGameObject.h"

GameObject::GameObject(std::string _uid)
{
	uid = _uid;
}

void GameObject::Start()
{
}

void GameObject::CleanUp()
{
}

void GameObject::SetName(const char * set)
{
	name = set;
}

const char * GameObject::GetName()
{
	return name.c_str();
}

std::string GameObject::GetUID()
{
	return uid;
}

void GameObject::SetParent(GameObject* set)
{
	if (set != parent && set != this)
	{
		if (IsInChildTree(set))
		{
			set->RemoveParent();
		}

		if (parent != nullptr)
		{
			for (std::vector<GameObject*>::iterator it = parent->childs.begin(); it != parent->childs.end(); ++it)
			{
				if ((*it) == this)
				{
					parent->childs.erase(it);
					break;
				}
			}
		}

		if (set != nullptr)
		{
			if (!set->IsChild(this))
			{
				set->childs.push_back(this);

				if (parent == nullptr)
					App->gameobject->RemoveGameObjectFromRoot(this);
			}
		}

		if (set == nullptr && parent != nullptr)
			App->gameobject->AddGameObjectToRoot(this);

		parent = set;
	}
}

void GameObject::RemoveParent()
{
	SetParent(nullptr);
}

uint GameObject::GetChildsCount() const
{
	return childs.size();
}

GameObject * GameObject::GetChild(uint index) const
{
	GameObject * ret = nullptr;

	if (childs.size() > index)
		ret = childs[index];

	return ret;
}

bool GameObject::IsChild(GameObject * go)
{
	bool ret = false;

	if (go != nullptr)
	{
		for (std::vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
		{
			if ((*it) == go)
			{
				ret = true;
				break;
			}
		}
	}

	return ret;
}

bool GameObject::IsInChildTree(GameObject* go)
{
	bool ret = false;

	if (go != nullptr)
	{
		std::vector<GameObject*> to_check = childs;

		while (!to_check.empty())
		{
			GameObject* curr = *to_check.begin();

			if (go == curr)
			{
				ret = true;
				break;
			}

			to_check.erase(to_check.begin());

			to_check.insert(to_check.begin(), curr->childs.begin(), curr->childs.end());
		}
	}

	return ret;
}

GameObject* GameObject::GetParent() const
{
	return parent;
}

bool GameObject::GetSelected() const
{
	return selected;
}
