#include "GameObject.h"
#include "App.h"
#include "ModuleGameObject.h"
#include "ComponentTransfrom.h"

GameObject::GameObject(std::string _uid)
{
	uid = _uid;
}

void GameObject::Start()
{
	transform = (ComponentTransform*)CreateComponent(ComponentType::TRANSFORM);
}

void GameObject::Update()
{

}

void GameObject::CleanUp()
{
	DestroyAllComponentsNow();
}

void GameObject::SetName(const char * set)
{
	name = set;
}

std::string GameObject::GetName()
{
	return name;
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

GameObject* GameObject::GetParent() const
{
	return parent;
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

std::vector<GameObject*> GameObject::GetChilds() const
{
	return childs;
}

GameObjectComponent* GameObject::CreateComponent(const ComponentType & type)
{
	GameObjectComponent* ret = nullptr;

	switch (type)
	{
	case ComponentType::TRANSFORM:
		ret = new ComponentTransform();
		break;
	default:
		break;
	}

	if (ret != nullptr)
	{
		if (ret->unique_per_game_object)
		{
			if (GetHasComponent(type) )
			{
				RELEASE(ret);
				ret = nullptr;
			}
		}
		
		if (ret != nullptr)
		{
			ret->owner = this;
			components.push_back(ret);
			ret->Start();
		}
	}

	return ret;
}

void GameObject::DestroyComponent(GameObjectComponent * component, bool check_can_destroy)
{
	if (component != nullptr)
	{
		if (component->can_destroy || !check_can_destroy)
		{
			for (std::vector<GameObjectComponent*>::iterator it = components.begin(); it != components.end(); ++it)
			{
				if ((*it) == component)
				{
					components.erase(it);
					break;
				}
			}

			bool exists = false;
			for (std::vector<GameObjectComponent*>::iterator it = components_to_destroy.begin(); it != components_to_destroy.end(); ++it)
			{
				if ((*it) == component)
				{
					exists = true;
					break;
				}
			}

			if (!exists)
			{
				components_to_destroy.push_back(component);
			}
		}
	}
}

bool GameObject::GetHasComponent(const ComponentType & type) const
{
	bool ret = false;

	for (std::vector<GameObjectComponent*>::const_iterator it = components.begin(); it != components.end(); ++it)
	{
		if ((*it)->GetType() == type)
		{
			ret = true;
			break;
		}
	}

	return ret;
}

std::vector<GameObjectComponent*> GameObject::GetComponents() const
{
	return components;
}

bool GameObject::GetSelected() const
{
	return selected;
}

void GameObject::DestroyAllComponentsNow()
{
	std::vector<GameObjectComponent*> all_components = components;

	for (std::vector<GameObjectComponent*>::const_iterator it = all_components.begin(); it != all_components.end(); ++it)
	{
		DestroyComponent(*it, false);
	}

	ActuallyDestroyComponents();
}

void GameObject::ActuallyDestroyComponents()
{
	for (std::vector<GameObjectComponent*>::iterator it = components_to_destroy.begin(); it != components_to_destroy.end(); ++it)
	{
		(*it)->CleanUp();
		RELEASE(*it);
	}

	components_to_destroy.clear();
}
