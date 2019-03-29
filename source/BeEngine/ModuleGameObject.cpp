#include "ModuleGameObject.h"
#include "Functions.h"
#include "ModuleEvent.h"
#include "Event.h"
#include "ModuleEvent.h"
#include "ComponentScript.h"
#include "Event.h"
#include "ModuleState.h"
#include "Scene.h"
#include "ComponentTransfrom.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

ModuleGameObject::ModuleGameObject() : Module()
{
	
}

ModuleGameObject::~ModuleGameObject()
{
}

bool ModuleGameObject::Awake()
{
	bool ret = true;

	base_instance_uid = GetUIDRandomHexadecimal();

	CreateRootScene();

	App->event->Suscribe(std::bind(&ModuleGameObject::OnEvent, this, std::placeholders::_1), EventType::EDITOR_GOES_TO_PLAY);
	App->event->Suscribe(std::bind(&ModuleGameObject::OnEvent, this, std::placeholders::_1), EventType::EDITOR_GOES_TO_IDLE);

	AddComponentType(ComponentType::COMPONENT_TYPE_SPRITE_RENDERER, "Sprite Renderer");
	AddComponentType(ComponentType::COMPONENT_TYPE_SCRIPT, "Script");
	AddComponentType(ComponentType::COMPONENT_TYPE_CAMERA, "Camera");
	AddComponentType(ComponentType::COMPONENT_TYPE_CANVAS, "UI Canvas");
	AddComponentType(ComponentType::COMPONENT_TYPE_BUTTON, "UI Button");
	AddComponentType(ComponentType::COMPONENT_TYPE_PHYSICS_BODY, "Physics Body");
	AddComponentType(ComponentType::COMPONENT_TYPE_POLYGON_COLLIDER, "Polygon Collider");

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

	UpdateGameObjects();

	return ret;
}

bool ModuleGameObject::Update()
{
	bool ret = true;

	UpdateGameObjectsLogic();

	return ret;
}

bool ModuleGameObject::PostUpdate()
{
	bool ret = true;

	ActuallyDestroyGameObjects();
	ActuallyDestroyScenes();

	return ret;
}

bool ModuleGameObject::CleanUp()
{
	bool ret = true;

	DestroyAllScenesNow();

	root_scene->CleanUp();
	RELEASE(root_scene);

	App->event->UnSuscribe(std::bind(&ModuleGameObject::OnEvent, this, std::placeholders::_1), EventType::EDITOR_GOES_TO_PLAY);

	return ret;
}

void ModuleGameObject::OnLoadProject(JSON_Doc * config)
{
}

void ModuleGameObject::OnSaveProject(JSON_Doc * config)
{
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

	case EventType::EDITOR_GOES_TO_IDLE:
	{
		needs_to_stop_logic = true;

		break;
	}
	}
}

GameObject* ModuleGameObject::CreateGameObject(Scene* scene)
{
	return CreateGameObject(scene, GetUIDRandomHexadecimal(), base_instance_uid);
}

GameObject * ModuleGameObject::CreateGameObject(Scene* scene, const std::string & uid, const std::string & instance_uid)
{
	GameObject* ret = nullptr;

	if (scene == nullptr)
		scene = root_scene;

	ret = new GameObject(uid, instance_uid);

	std::string name = "GameObject" + std::to_string(game_objects.size());
	ret->SetName(name.c_str());

	game_objects.push_back(ret);

	SetGameObjectScene(scene, ret);
	AddGameObjectToRoot(ret);

	ret->Start();

	EventGameObjectCreated* e_go = new EventGameObjectCreated(ret);
	App->event->SendEvent(e_go);

	RemoveAllGameObjectsFromSelected();
	AddGameObjectToSelected(ret);

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
				curr->DestroyAllComponents();

				game_objects_to_destroy.push_back(curr);
			}

			if(curr->GetSelected())
				RemoveGameObjectFromSelected(curr);			

			if (curr->GetParent() == nullptr)
				RemoveGameObjectFromRoot(curr);

			to_check.erase(to_check.begin());

			to_check.insert(to_check.begin(), curr->childs.begin(), curr->childs.end());
		}
	}
}

void ModuleGameObject::DuplicateGameObjects(std::vector<GameObject*> gos)
{
	std::vector<GameObject*> to_duplicate;
}

std::vector<GameObject*> ModuleGameObject::GetGameObjects()
{
	return game_objects;
}

GameObject * ModuleGameObject::GetGameObjectByUID(const std::string & uid, const std::string & instance_uid)
{
	GameObject* ret = nullptr;

	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		if ((*it)->GetUID().compare(uid) == 0 && (*it)->GetInstanceUID().compare(instance_uid) == 0)
		{
			ret = *it;
			break;
		}
	}

	return ret;
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
			go->CallOnGameObjectSelected();

			game_objects_selected.push_back(go);
		}
	}
}

void ModuleGameObject::AddGameObjectAsOnlySelected(GameObject * go)
{
	if (go != nullptr)
	{
		RemoveAllGameObjectsFromSelected();
		AddGameObjectToSelected(go);
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
				go->CallOnGameObjectDeSelected();

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
		(*it)->CallOnGameObjectDeSelected();
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

Scene* ModuleGameObject::CreateSubScene()
{
	Scene* sub_scene = new Scene(GetUIDRandomHexadecimal());
	sub_scene->Start();
	sub_scene->SetName("Sub Scene");

	sub_scenes.push_back(sub_scene);

	return sub_scene;
}

void ModuleGameObject::DestroyScene(Scene * scene)
{
	if (scene != nullptr)
	{
		if (scene == root_scene)
		{
			std::vector<GameObject*> gos = scene->GetRootGameObjects();

			for (std::vector<GameObject*>::iterator it = gos.begin(); it != gos.end(); ++it)
			{
				DestroyGameObject((*it));
			}

			scene->root_game_objects.clear();

			while (sub_scenes.size() > 0)
			{
				DestroyScene(*sub_scenes.begin());
			}
		}
		else
		{
			std::vector<GameObject*> gos = scene->GetRootGameObjects();

			for (std::vector<GameObject*>::iterator it = gos.begin(); it != gos.end(); ++it)
			{
				DestroyGameObject((*it));
			}

			scene->root_game_objects.clear();

			for (std::vector<Scene*>::iterator it = sub_scenes.begin(); it != sub_scenes.end(); ++it)
			{
				if ((*it) == scene)
				{
					sub_scenes.erase(it);
					break;
				}
			}

			bool found = false;
			for (std::vector<Scene*>::iterator it = scenes_to_destroy.begin(); it != scenes_to_destroy.end(); ++it)
			{
				if ((*it) == scene)
				{
					found = true;
					break;
				}
			}

			if (scene->selected)
				RemoveSceneFromSelected(scene);

			if (!found)
			{
				scenes_to_destroy.push_back(scene);
			}
		}
	}
}

Scene * ModuleGameObject::GetRootScene() const
{
	return root_scene;
}

std::vector<Scene*> ModuleGameObject::GetSubScenes() const
{
	return sub_scenes;
}

void ModuleGameObject::AddSceneToSelected(Scene * scene)
{
	if (scene != nullptr && scene != root_scene)
	{
		bool found = false;
		for (std::vector<Scene*>::iterator it = sub_scenes_selected.begin(); it != sub_scenes_selected.end(); ++it)
		{
			if (scene == (*it))
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			scene->selected = true;

			sub_scenes_selected.push_back(scene);
		}
	}
}

void ModuleGameObject::RemoveSceneFromSelected(Scene * scene)
{
	if (scene != nullptr)
	{
		for (std::vector<Scene*>::iterator it = sub_scenes_selected.begin(); it != sub_scenes_selected.end(); ++it)
		{
			if (scene == (*it))
			{
				scene->selected = false;

				sub_scenes_selected.erase(it);
				break;
			}
		}
	}
}

void ModuleGameObject::RemoveAllScenesFromSelected()
{
	for (std::vector<Scene*>::iterator it = sub_scenes_selected.begin(); it != sub_scenes_selected.end(); ++it)
	{
		(*it)->selected = false;
	}

	sub_scenes_selected.clear();
}

std::vector<Scene*> ModuleGameObject::GetSelectedScenes() const
{
	return sub_scenes_selected;
}

void ModuleGameObject::SetGameObjectScene(Scene * scene, GameObject* go)
{
	if (scene != nullptr && go != nullptr)
	{
		std::vector<GameObject*> to_check;

		to_check.push_back(go);

		while (to_check.size() > 0)
		{
			GameObject* curr_go = *to_check.begin();

			if (go->scene != nullptr)
			{
				if (go->GetParent() == nullptr)
					RemoveGameObjectFromRoot(go);
			}

			go->scene = scene;

			if (go->GetParent() == nullptr)
				AddGameObjectToRoot(go);

			to_check.erase(to_check.begin());

			to_check.insert(to_check.begin(), curr_go->childs.begin(), curr_go->childs.end());
		}
	}
}

void ModuleGameObject::AddGameObjectToRoot(GameObject * go)
{
	if (go != nullptr &&  go->scene != nullptr)
	{
		bool exists = false;

		for (std::vector<GameObject*>::iterator it = go->scene->root_game_objects.begin(); it != go->scene->root_game_objects.end(); ++it)
		{
			if ((*it) == go)
			{
				exists = true;
				break;
			}
		}

		if (!exists)
		{
			go->scene->root_game_objects.push_back(go);
		}
	}
}

void ModuleGameObject::RemoveGameObjectFromRoot( GameObject * go)
{
	if (go != nullptr &&  go->scene != nullptr)
	{
		for (std::vector<GameObject*>::iterator it = go->scene->root_game_objects.begin(); it != go->scene->root_game_objects.end(); ++it)
		{
			if ((*it) == go)
			{
				go->scene->root_game_objects.erase(it);
				break;
			}
		}
	}
}

void ModuleGameObject::ChangeGameObjectPositionOnRootList(GameObject * go, uint new_pos)
{
	if (go != nullptr &&  go->scene != nullptr)
	{
		if (go->GetParent() == nullptr)
		{
			bool exists = false;
			uint last_pos = 0;

			for (std::vector<GameObject*>::iterator it = go->scene->root_game_objects.begin(); it != go->scene->root_game_objects.end(); ++it)
			{
				if ((*it) == go)
				{
					go->scene->root_game_objects.erase(it);
					exists = true;
					break;
				}

				++last_pos;
			}

			if (exists)
			{
				if (last_pos < new_pos)
					--new_pos;

				if (new_pos > go->scene->root_game_objects.size())
					new_pos = go->scene->root_game_objects.size();

				if (new_pos < 0)
					new_pos = 0;

				go->scene->root_game_objects.insert(go->scene->root_game_objects.begin() + new_pos, go);
			}
		}
	}
}

void ModuleGameObject::ChangeGameObjectPositionOnParentChildren(GameObject * go, uint new_pos)
{
	if (go != nullptr)
	{
		GameObject* parent = go->GetParent();

		if (parent != nullptr)
		{
			go->scene = parent->scene;

			bool exists = false;
			uint last_pos = 0;

			for (std::vector<GameObject*>::iterator it = parent->childs.begin(); it != parent->childs.end(); ++it)
			{
				if ((*it) == go)
				{
					parent->childs.erase(it);
					exists = true;
					break;
				}

				++last_pos;
			}

			if (exists)
			{
				if (last_pos < new_pos)
					--new_pos;

				if (new_pos > parent->childs.size())
					new_pos = parent->childs.size();

				if (new_pos < 0)
					new_pos = 0;

				parent->childs.insert(parent->childs.begin() + new_pos, go);
			}
		}
	}
}

void ModuleGameObject::ChangeScenePositionOnList(Scene * scene, uint new_pos)
{
	if (scene != nullptr)
	{
		bool exists = false;
		uint last_pos = 0;

		for (std::vector<Scene*>::iterator it = sub_scenes.begin(); it != sub_scenes.end(); ++it)
		{
			if ((*it) == scene)
			{
				sub_scenes.erase(it);
				exists = true;
				break;
			}

			++last_pos;
		}

		if (exists)
		{
			if (last_pos < new_pos)
				--new_pos;

			if (new_pos > sub_scenes.size())
				new_pos = sub_scenes.size();

			if (new_pos < 0)
				new_pos = 0;

			sub_scenes.insert(sub_scenes.begin() + new_pos, scene);
		}
	}
}

std::map<ComponentType, std::string> ModuleGameObject::GetComponentsTypes() const
{
	return components_type;
}

void ModuleGameObject::AddComponentType(const ComponentType & type, const std::string& name)
{
	components_type[type] = name;
}

void ModuleGameObject::CreateRootScene()
{
	root_scene = new Scene(GetUIDRandomHexadecimal());
	root_scene->Start();
	root_scene->SetName("undefined");
}

void ModuleGameObject::MergeScenes()
{
}

void ModuleGameObject::UpdateGameObjects()
{
	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		GameObject* curr_go = (*it);

		if (curr_go->GetActive())
		{
			curr_go->Update();

			for (std::vector<GameObjectComponent*>::iterator co = curr_go->components.begin(); co != curr_go->components.end(); ++co)
			{
				GameObjectComponent* curr_component = *co;

				curr_component->Update();
			}
		}
	}

	for (std::vector<GameObject*>::iterator it = game_objects.begin(); it != game_objects.end(); ++it)
	{
		(*it)->ActuallyDestroyComponents();
	}
}

void ModuleGameObject::ActuallyDestroyGameObjects()
{
	for (std::vector<GameObject*>::iterator it = game_objects_to_destroy.begin(); it != game_objects_to_destroy.end(); ++it)
	{
		GameObject* curr_go = (*it);

		std::vector<GameObject*> childs = curr_go->childs;

		for (std::vector<GameObject*>::iterator ch = childs.begin(); ch != childs.end(); ++ch)
		{
			(*ch)->scene = nullptr;
			(*ch)->RemoveParent();
		}

		curr_go->scene = nullptr;
		curr_go->RemoveParent();

		curr_go->CleanUp();
		RELEASE(curr_go);
	}

	game_objects_to_destroy.clear();
}

void ModuleGameObject::ActuallyDestroyScenes()
{
	for (std::vector<Scene*>::iterator it = scenes_to_destroy.begin(); it != scenes_to_destroy.end(); ++it)
	{
		Scene* curr_scene = (*it);

		curr_scene->CleanUp();
		RELEASE(curr_scene);
	}

	scenes_to_destroy.clear();
}

void ModuleGameObject::DestroyAllScenesNow()
{
	DestroyScene(root_scene);

	ActuallyDestroyGameObjects();
	ActuallyDestroyScenes();
}

void ModuleGameObject::UpdateGameObjectsLogic()
{	
	if (needs_to_start_logic)
	{
		if (App->state->GetEditorUpdateState() != EditorUpdateState::EDITOR_UPDATE_STATE_IDLE)
		{
			SaveSceneEditorPlay();

			GameObjectsLogicStart();
		}

		needs_to_start_logic = false;
	}

	if (App->state->GetEditorUpdateState() != EditorUpdateState::EDITOR_UPDATE_STATE_IDLE)
	{
		GameObjectsLogicUpdate();
	}
	
	if (needs_to_stop_logic)
	{
		if (App->state->GetEditorUpdateState() == EditorUpdateState::EDITOR_UPDATE_STATE_IDLE)
		{
			GameObjectsLogicStop();

			LoadSceneEditorPlay();
		}

		needs_to_stop_logic = false;
	}
	
}

void ModuleGameObject::GameObjectsLogicStart()
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

				script->InitFields();
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

void ModuleGameObject::GameObjectsLogicStop()
{

}

void ModuleGameObject::SaveSceneEditorPlay()
{
	editor_play_sub_scenes_abs.clear();

	std::vector<GameObject*> root_gos = root_scene->GetRootGameObjects();

	editor_play_scene_abs.Abstract(root_gos);

	for (std::vector<Scene*>::iterator it = sub_scenes.begin(); it != sub_scenes.end(); ++it)
	{
		GameObjectAbstraction sub_abs;
		
		std::vector<GameObject*> root_sub_gos = (*it)->GetRootGameObjects();

		sub_abs.Abstract(root_sub_gos);

		editor_play_sub_scenes_abs.push_back(sub_abs);
	}
}

void ModuleGameObject::LoadSceneEditorPlay()
{
	DestroyScene(root_scene);

	editor_play_scene_abs.DeAbstract();

	for (std::vector<GameObjectAbstraction>::iterator it = editor_play_sub_scenes_abs.begin(); it != editor_play_sub_scenes_abs.end(); ++it)
	{
		Scene* new_scene = CreateSubScene();

		std::vector<GameObject*> new_gos = (*it).DeAbstract();

		for (std::vector<GameObject*>::iterator it = new_gos.begin(); it != new_gos.end(); ++it)
		{
			if ((*it)->GetParent() == nullptr)
			{
				SetGameObjectScene(new_scene, (*it));
			}
		}
	}
}
