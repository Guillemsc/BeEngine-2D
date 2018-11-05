#ifndef __MODULE_GAME_OBJECT_H__
#define __MODULE_GAME_OBJECT_H__

#include "Module.h"
#include "GameObject.h"

class ModuleGameObject : public Module
{
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

private:
	void DestroyAllGameObjects();

	void ActuallyDestroyGameObjects();

private:
	std::vector<GameObject*> game_objects;
	std::vector<GameObject*> game_objects_to_destroy;
};

#endif // !__MODULE_GAME_OBJECT_H__