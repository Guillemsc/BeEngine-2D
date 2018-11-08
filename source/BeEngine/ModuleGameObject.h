#ifndef __MODULE_GAME_OBJECT_H__
#define __MODULE_GAME_OBJECT_H__

#include "Module.h"
#include "GameObject.h"

class ModuleGameObject : public Module
{
	friend class GameObject;

public:
	ModuleGameObject();
	~ModuleGameObject();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

	GameObject* CreateGameObject();
	void DestroyGameObject(GameObject* go);

	GameObject* GetGameObjectByUID(const char* uid);

	std::vector<GameObject*> GetRootGameObjects() const;

	void AddGameObjectToSelected(GameObject* go);
	void RemoveGameObjectFromSelected(GameObject* go);
	void RemoveAllGameObjectsFromSelected();
	std::vector<GameObject*> GetSelectedGameObjects() const;

private:
	void DestroyAllGameObjects();
	void ActuallyDestroyGameObjects();

	void AddGameObjectToRoot(GameObject* go);
	void RemoveGameObjectFromRoot(GameObject* go);

private:
	std::vector<GameObject*> game_objects;
	std::vector<GameObject*> game_objects_to_destroy;
	std::vector<GameObject*> game_objects_root;
	std::vector<GameObject*> game_objects_selected;
};

#endif // !__MODULE_GAME_OBJECT_H__