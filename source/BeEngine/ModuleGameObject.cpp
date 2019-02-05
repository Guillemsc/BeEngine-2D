#include "ModuleGameObject.h"
#include "Functions.h"
#include "ModuleEvent.h"
#include "Event.h"
#include "ModuleEvent.h"
#include "ComponentScript.h"
#include "Event.h"
#include "ModuleState.h"

ModuleGameObject::ModuleGameObject()
{
}

ModuleGameObject::~ModuleGameObject()
{
}

bool ModuleGameObject::Awake()
{
	bool ret = true;

	App->event->Suscribe(std::bind(&ModuleGameObject::OnEvent, this, std::placeholders::_1), EventType::EDITOR_GOES_TO_PLAY);

	AddComponentType(ComponentType::COMPONENT_TYPE_SPRITE_RENDERER, "Sprite Renderer");
	AddComponentType(ComponentType::COMPONENT_TYPE_SCRIPT, "Script");

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

	UpdateGameObjects();

	UpdateGameObjectsLogic();

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

	DestroyAllGameObjectsNow();

	App->event->UnSuscribe(std::bind(&ModuleGameObject::OnEvent, this, std::placeholders::_1), EventType::EDITOR_GOES_TO_PLAY);

	return ret;
}

void ModuleGameObject::OnEvent(Event * ev)
{
	switch (ev->GetType())
	{
	case EventType::EDITOR_GOES_TO_PLAY:
	{
		needs_to_start_logic = true;

		break;
	}
	}
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

	EventGameObjectCreated* e_go = new EventGameObjectCreated(ret);
	App->event->SendEvent(e_go);

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

			EventGameObjectDestroyed* egod = new EventGameObjectDestroyed(curr);
			App->event->SendEvent(egod);

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

std::vector<GameObject*> ModuleGameObject::GetGameObjects()
{
	return game_objects;
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

std::map<ComponentType, std::string> ModuleGameObject::GetComponentsTypes() const
{
	return components_type;
}

void ModuleGameObject::AddComponentType(const ComponentType & type, const std::string& name)
{
	components_type[type] = name;
}

void ModuleGameObject::UpdateGameObjects()
{
	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		(*it)->Update();
	}

	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		(*it)->ActuallyDestroyComponents();
	}
}

void ModuleGameObject::DestroyAllGameObjectsNow()
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
		GameObject* curr_go = (*it);

		std::vector<GameObject*> childs = curr_go->childs;

		for (std::vector<GameObject*>::iterator ch = childs.begin(); ch != childs.end(); ++ch)
		{
			(*ch)->RemoveParent();
		}

		curr_go->RemoveParent();

		RemoveGameObjectFromRoot(curr_go);

		curr_go->CleanUp();
		RELEASE(curr_go);
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

void ModuleGameObject::UpdateGameObjectsLogic()
{
	if (App->state->GetEditorUpdateState() != EditorUpdateState::EDITOR_UPDATE_STATE_IDLE)
	{
		if (needs_to_start_logic)
		{
			GameObjectsLogicPlay();
			needs_to_start_logic = false;
		}

		GameObjectsLogicUpdate();
	}
}

void ModuleGameObject::GameObjectsLogicPlay()
{
	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		GameObject* curr_go = *it;

		std::vector<GameObjectComponent*> components = curr_go->GetComponents();

		for (std::vector<GameObjectComponent*>::iterator co = components.begin(); co != components.end(); ++co)
		{
			GameObjectComponent* curr_component = *co;

			if (curr_component->GetType() == ComponentType::COMPONENT_TYPE_SCRIPT)
			{
				ComponentScript* script = (ComponentScript*)curr_component;

				script->DestroyScriptInstance();
				script->CreateScriptInstance();
			}
		}
	}

	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		GameObject* curr_go = *it;

		std::vector<GameObjectComponent*> components = curr_go->GetComponents();

		for (std::vector<GameObjectComponent*>::iterator co = components.begin(); co != components.end(); ++co)
		{
			GameObjectComponent* curr_component = *co;

			if (curr_component->GetType() == ComponentType::COMPONENT_TYPE_SCRIPT)
			{
				ComponentScript* script = (ComponentScript*)curr_component;

				script->CallAwake();
			}
		}
	}

	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		GameObject* curr_go = *it;

		std::vector<GameObjectComponent*> components = curr_go->GetComponents();

		for (std::vector<GameObjectComponent*>::iterator co = components.begin(); co != components.end(); ++co)
		{
			GameObjectComponent* curr_component = *co;

			if (curr_component->GetType() == ComponentType::COMPONENT_TYPE_SCRIPT)
			{
				ComponentScript* script = (ComponentScript*)curr_component;

				script->CallStart();
			}
		}
	}
}

void ModuleGameObject::GameObjectsLogicUpdate()
{
	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		GameObject* curr_go = *it;

		std::vector<GameObjectComponent*> components = curr_go->GetComponents();

		for (std::vector<GameObjectComponent*>::iterator co = components.begin(); co != components.end(); ++co)
		{
			GameObjectComponent* curr_component = *co;

			if (curr_component->GetType() == ComponentType::COMPONENT_TYPE_SCRIPT)
			{
				ComponentScript* script = (ComponentScript*)curr_component;

				script->CallUpdate();
			}
		}
	}
}
