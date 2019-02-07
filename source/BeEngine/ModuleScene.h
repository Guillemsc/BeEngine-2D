#ifndef __MODULE_SCENE_H__
#define __MODULE_SCENE_H__

#include "Module.h"

class ModuleScene : public Module
{
public:
	ModuleScene();
	~ModuleScene();

	bool Awake();
	bool CleanUp();

};

#endif // !__MODULE_SCENE_H__