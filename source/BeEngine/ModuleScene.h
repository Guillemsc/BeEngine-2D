#ifndef __MODULE_SCENE_H__
#define __MODULE_SCENE_H__

#include "Module.h"

class GameObject;
class ResourceScene;

class ModuleScene : public Module
{
public:
	ModuleScene();
	~ModuleScene();

	bool Awake();
	bool CleanUp();

	bool CreateNewPrefab(GameObject* go);
	bool UpdatePrefab(GameObject* go);
	bool UpdateFromPrefab(GameObject* go);
};

#endif // !__MODULE_SCENE_H__