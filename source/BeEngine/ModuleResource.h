#ifndef __MODULE_RESOURCE_H__
#define __MODULE_RESOURCE_H__

#include "Module.h"

class ModuleResource: public Module
{
public:
	ModuleResource();
	~ModuleResource();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

};

#endif // !__MODULE_RESOURCE_H__