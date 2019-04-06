#include "GameObject.h"
#include "App.h"
#include "ModuleGameObject.h"
#include "ComponentTransfrom.h"
#include "ComponentSpriteRenderer.h"
#include "ComponentScript.h"
#include "ModuleResource.h"
#include "ComponentCamera.h"
#include "ModuleJson.h"
#include "ResourcePrefab.h"
#include "ComponentPhysicsBody.h"
#include "ComponentPolygonCollider.h"
#include "ComponentCanvas.h"
#include "Event.h"
#include "ModuleEvent.h"
#include "Scene.h"
#include "ScriptingBridgeGameObject.h"
#include "ComponentButton.h"
#include "ComponentText.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

GameObject::GameObject(const std::string & set_uid, const std::string & set_instance_uid) :
	BeObject(new ScriptingBridgeGameObject(this))
{
	uid = set_uid;
	instance_uid = set_instance_uid;

	uid_plus_instance_uid = uid + instance_uid;
}

void GameObject::OnSaveAbstraction(DataAbstraction & abs)
{
	abs.Clear();

	abs.AddString("name", name);
	abs.AddBool("active", active);

	if(resource_prefab != nullptr)
		abs.AddString("resource_prefab", resource_prefab->GetUID());
}

void GameObject::OnLoadAbstraction(DataAbstraction & abs)
{
	name = abs.GetString("name");
	active = abs.GetBool("active", true);

	std::string prefab_uid = abs.GetString("resource_prefab");

	if (prefab_uid.compare("") != 0)
		resource_prefab = (ResourcePrefab*)App->resource->GetResourceFromUid(prefab_uid, ResourceType::RESOURCE_TYPE_PREFAB);
}

void GameObject::Start()
{
	InitBeObject();

	transform = (ComponentTransform*)CreateComponent(ComponentType::COMPONENT_TYPE_TRANSFORM);

	App->event->Suscribe(std::bind(&GameObject::OnEvent, this, std::placeholders::_1), EventType::RESOURCE_DESTROYED);
}

void GameObject::Update()
{

}

void GameObject::CleanUp()
{
	ActuallyDestroyComponents();

	CleanUpBeObject();
}

void GameObject::OnEvent(Event * ev)
{
	if (ev->GetType() == EventType::RESOURCE_DESTROYED)
	{
		EventResourceDestroyed* erd = (EventResourceDestroyed*)ev;

		if (erd->GetResource() == resource_prefab)
			resource_prefab = nullptr;
	}
}

void GameObject::SetName(const char * set)
{
	name = set;
}

std::string GameObject::GetName()
{
	return name;
}

void GameObject::SetActive(bool set)
{
	std::vector<GameObject*> to_check;

	to_check.push_back(this);

	while (to_check.size() > 0)
	{
		GameObject* curr_go = *to_check.begin();

		bool last_active = curr_go->active;

		curr_go->active = set;

		if (last_active != set)
			curr_go->CallOnChangeActive(set);

		std::vector<GameObject*> childs = curr_go->GetChilds();

		to_check.erase(to_check.begin());

		to_check.insert(to_check.begin(), childs.begin(), childs.end());
	}
}

bool GameObject::GetActive() const
{
	return active;
}

std::string GameObject::GetUID() const
{
	return uid;
}

std::string GameObject::GetInstanceUID() const
{
	return instance_uid;
}

std::string GameObject::GetUIDPlusInstanceUID() const
{
	return uid_plus_instance_uid;
}

Scene * GameObject::GetScene() const
{
	return scene;
}

void GameObject::SetParent(GameObject* set)
{
	bool wrong_scene = false;

	if (set != nullptr && set->GetScene() != scene)
		wrong_scene = true;

	if (!wrong_scene)
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

			float4x4 last_trans = transform->GetWorldTransform();

			parent = set;

			transform->SetWorldTransform(last_trans);
		}
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

uint GameObject::GetChildDeepness()
{
	uint ret = 0;

	GameObject* curr_parent = parent;

	while (curr_parent != nullptr)
	{
		curr_parent = curr_parent->parent;

		++ret;
	}

	return ret;
}

GameObjectComponent* GameObject::CreateComponent(const ComponentType & type)
{
	GameObjectComponent* ret = nullptr;

	switch (type)
	{
	case ComponentType::COMPONENT_TYPE_TRANSFORM:
		ret = new ComponentTransform();
		break;

	case ComponentType::COMPONENT_TYPE_SPRITE_RENDERER:
		ret = new ComponentSpriteRenderer();
		break;

	case ComponentType::COMPONENT_TYPE_SCRIPT:
		ret = new ComponentScript();
		break;

	case ComponentType::COMPONENT_TYPE_CAMERA:
		ret = new ComponentCamera();
		break;

	case ComponentType::COMPONENT_TYPE_CANVAS:
		ret = new ComponentCanvas();
		break;

	case ComponentType::COMPONENT_TYPE_PHYSICS_BODY:
		ret = new ComponentPhysicsBody();
		break;

	case ComponentType::COMPONENT_TYPE_POLYGON_COLLIDER:
		ret = new ComponentPolygonCollider();
		break;

	case ComponentType::COMPONENT_TYPE_BUTTON:
		ret = new ComponentButton();
		break;

	case ComponentType::COMPONENT_TYPE_TEXT:
		ret = new ComponentText();
		break;

	default:
		break;
	}

	if (ret != nullptr)
	{
		if (ret->unique_per_game_object)
		{
			if (GetHasComponent(type))
			{
				RELEASE(ret);
				ret = nullptr;
			}
		}
		
		if (ret != nullptr)
		{
			ret->owner = this;
			ret->uid = App->resource->GetNewUID();
			components.push_back(ret);
			ret->Start();

			CallOnAddComponent(ret);
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
				component->OnDestroy();

				CallOnRemoveComponent(component);

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

GameObjectComponent* GameObject::GetComponent(ComponentType type) const
{
	return GetComponent(type, 0);
}

GameObjectComponent * GameObject::GetComponent(ComponentType type, int index) const
{
	GameObjectComponent* ret = nullptr;

	int counter = 0;
	for (std::vector<GameObjectComponent*>::const_iterator it = components.begin(); it != components.end(); ++it)
	{
		if ((*it)->GetType() == type)
		{
			if (counter == index)
			{
				ret = (*it);
				break;
			}

			++counter;
		}
	}

	return ret;
}

ComponentScript * GameObject::GetComponentScript(std::string script_name, int index)
{
	ComponentScript* ret = nullptr;

	int counter = 0;
	for (std::vector<GameObjectComponent*>::const_iterator it = components.begin(); it != components.end(); ++it)
	{
		if ((*it)->GetType() == ComponentType::COMPONENT_TYPE_SCRIPT)
		{
			ComponentScript* c_script = (ComponentScript*)(*it);

			if (c_script->GetName().compare(script_name))
			{
				if (counter == index)
				{
					ret = c_script;
					break;
				}

				++counter;
			}
		}
	}

	return ret;
}

bool GameObject::GetSelected() const
{
	return selected;
}

bool GameObject::GetHasPrefab() const
{
	return resource_prefab != nullptr;
}

ResourcePrefab * GameObject::GetPrefab() const
{
	return resource_prefab;
}

void GameObject::DestroyAllComponents()
{
	std::vector<GameObjectComponent*> all_components = components;

	for (std::vector<GameObjectComponent*>::const_iterator it = all_components.begin(); it != all_components.end(); ++it)
	{
		DestroyComponent(*it, false);
	}

}

void GameObject::ActuallyDestroyComponents()
{
	for (std::vector<GameObjectComponent*>::iterator it = components_to_destroy.begin(); it != components_to_destroy.end(); ++it)
	{
		(*it)->CleanUp();
	}

	for (std::vector<GameObjectComponent*>::iterator it = components_to_destroy.begin(); it != components_to_destroy.end(); ++it)
	{
		RELEASE(*it);
	}

	components_to_destroy.clear();
}

void GameObject::CallOnAddComponent(GameObjectComponent * comp)
{
	for (std::vector<GameObjectComponent*>::const_iterator it = components.begin(); it != components.end(); ++it)
	{
		(*it)->OnAddComponent(comp);
	}
}

void GameObject::CallOnRemoveComponent(GameObjectComponent * comp)
{
	for (std::vector<GameObjectComponent*>::const_iterator it = components.begin(); it != components.end(); ++it)
	{
		(*it)->OnRemoveComponent(comp);
	}
}

void GameObject::CallOnGameObjectSelected()
{
	for (std::vector<GameObjectComponent*>::const_iterator it = components.begin(); it != components.end(); ++it)
	{
		(*it)->OnOwnerSelected();
	}
}

void GameObject::CallOnGameObjectDeSelected()
{
	for (std::vector<GameObjectComponent*>::const_iterator it = components.begin(); it != components.end(); ++it)
	{
		(*it)->OnOwnerDeSelected();
	}
}

void GameObject::CallOnChangeActive(bool active)
{
	for (std::vector<GameObjectComponent*>::const_iterator it = components.begin(); it != components.end(); ++it)
	{
		(*it)->OnChangeActive(active);
	}
}
