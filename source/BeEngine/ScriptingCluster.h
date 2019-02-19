#ifndef __SCRIPTING_CLUSTER_H__
#define __SCRIPTING_CLUSTER_H__

#include "ModuleScripting.h"

class ScriptingCluster 
{
	friend class ModuleScripting;

private:
	void operator delete(void *) {}

public:
	void RegisterInternalCalls();
	void RebuildClasses();

public:
	ScriptingClass beengine_object_class;
	ScriptingClass game_object_class;

private:
	bool rebuild_internal_calls = true;
};

#endif // !__SCRIPTING_CLUSTER_H__