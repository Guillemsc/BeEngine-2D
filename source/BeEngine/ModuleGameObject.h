#ifndef __MODULE_GAME_OBJECT_H__
#define __MODULE_GAME_OBJECT_H__

#include <map>

#include "Module.h"
#include "GameObject.h"
#include "GameObjectComponent.h"
#include "GameObjectAbstraction.h"

class Event;
class Scene;
class ComponentCanvas;

class GameObjectComponentData
{
public:
	GameObjectComponentData(const std::string& name, ComponentType type, const std::string scripting_name);

	std::string GetName() const;
	ComponentType GetType() const;
	std::string GetScriptingName() const;

private:
	std::string name;
	ComponentType type;
	std::string scripting_name;
};

class ModuleGameObject : public Module
{
	friend class GameObject;
	friend class HierarchyWindow;

public:
	ModuleGameObject();
	~ModuleGameObject();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

	void OnLoadProject(JSON_Doc* config);
	void OnSaveProject(JSON_Doc* config);

	void OnEvent(Event* ev);

	GameObject* CreateGameObject(Scene* scene = nullptr);
	GameObject* CreateGameObject(Scene* scene, const std::string& uid, const std::string& instance_uid);
	void DestroyGameObject(GameObject* go);
	void DuplicateGameObjects(std::vector<GameObject*> gos);
	std::vector<GameObject*> GetGameObjects();

	GameObject* GetGameObjectByUID(const std::string& uid, const std::string& instance_uid);

	void AddGameObjectToSelected(GameObject* go);
	void AddGameObjectAsOnlySelected(GameObject* go);
	void RemoveGameObjectFromSelected(GameObject* go);
	void RemoveAllGameObjectsFromSelected();
	std::vector<GameObject*> GetSelectedGameObjects() const;
	uint GetSelectedGameObjectsCount() const;

	Scene* CreateSubScene();
	void DestroyScene(Scene* scene);
	Scene* GetRootScene() const;
	std::vector<Scene*> GetSubScenes() const;

	void AddSceneToSelected(Scene* scene);
	void RemoveSceneFromSelected(Scene* go);
	void RemoveAllScenesFromSelected();
	std::vector<Scene*> GetSelectedScenes() const;

	void SetGameObjectScene(Scene* scene, GameObject* go);
	void AddGameObjectToRoot(GameObject* go);
	void RemoveGameObjectFromRoot(GameObject* go);
	void ChangeGameObjectPositionOnRootList(GameObject * go, uint new_pos);
	void ChangeGameObjectPositionOnParentChildren(GameObject* go, uint new_pos);
	void ChangeScenePositionOnList(Scene* scene, uint new_pos);

	std::vector<GameObjectComponentData> GetComponentsData() const;
	ComponentType GetComponentTypeByComponentScriptingName(const std::string& name);

	void AddComponentScript(ComponentScript* sc);
	void RemoveComponentScript(ComponentScript* sc);

private:
	void AddComponentType(const ComponentType& type, const std::string& name, const std::string& scripting_name);

	void CreateRootScene();
	void MergeScenes();

	void UpdateGameObjects();

	void ActuallyDestroyGameObjects();
	void ActuallyDestroyScenes();
	void DestroyAllScenesNow();

	void UpdateGameObjectsLogic();
	void GameObjectsLogicStart();
	void GameObjectsLogicUpdate();
	void GameObjectsLogicStop();

	void SaveSceneEditorPlay();
	void LoadSceneEditorPlay();

private:
	std::string base_instance_uid;

	std::vector<GameObject*> game_objects;
	std::vector<GameObject*> game_objects_to_destroy;
	std::vector<GameObject*> game_objects_selected;

	Scene* root_scene;
	std::vector<Scene*> sub_scenes;
	std::vector<Scene*> scenes_to_destroy;
	std::vector<Scene*> sub_scenes_selected;

	std::vector<GameObjectComponentData> components_data;

	bool needs_to_start_logic = false;
	bool needs_to_stop_logic = false;

	std::vector<ComponentScript*> component_scripts;
	std::vector<ComponentScript*> component_scripts_to_add;

	GameObjectAbstraction editor_play_scene_abs;
	std::vector<GameObjectAbstraction> editor_play_sub_scenes_abs;

	std::string last_scene_resource;
};

#endif // !__MODULE_GAME_OBJECT_H__