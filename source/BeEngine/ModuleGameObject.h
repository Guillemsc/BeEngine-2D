#ifndef __MODULE_GAME_OBJECT_H__
#define __MODULE_GAME_OBJECT_H__

#include <map>

#include "Module.h"
#include "GameObject.h"
#include "GameObjectComponent.h"

class Event;

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

	void OnEvent(Event* ev);

	GameObject* CreateGameObject();
	void DestroyGameObject(GameObject* go);
	std::vector<GameObject*> GetGameObjects();

	GameObject* GetGameObjectByUID(const char* uid);

	std::vector<GameObject*> GetRootGameObjects() const;

	void AddGameObjectToSelected(GameObject* go);
	void RemoveGameObjectFromSelected(GameObject* go);
	void RemoveAllGameObjectsFromSelected();
	std::vector<GameObject*> GetSelectedGameObjects() const;
	uint GetSelectedGameObjectsCount() const;

	std::map<ComponentType, std::string> GetComponentsTypes() const;

private:
	void AddComponentType(const ComponentType& type, const std::string& name);

	void UpdateGameObjects();

	void DestroyAllGameObjectsNow();
	void ActuallyDestroyGameObjects();

	void AddGameObjectToRoot(GameObject* go);
	void RemoveGameObjectFromRoot(GameObject* go);

	void ChangeGameObjectPositionOnRootList(GameObject* go, uint new_pos);
	void ChangeGameObjectPositionOnParentChildren(GameObject* go, uint new_pos);

	void GameObjectsPlay();

private:
	std::vector<GameObject*> game_objects;
	std::vector<GameObject*> game_objects_to_destroy;
	std::vector<GameObject*> game_objects_root;
	std::vector<GameObject*> game_objects_selected;

	std::map<ComponentType, std::string> components_type;
};

#endif // !__MODULE_GAME_OBJECT_H__