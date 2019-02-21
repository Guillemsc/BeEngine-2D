#ifndef __SCRIPTING_CLUSTER_H__
#define __SCRIPTING_CLUSTER_H__

#include "ModuleScripting.h"

class ScriptingCluster 
{
	friend class ModuleScripting;

private:
	void operator delete(void *) {}

private:
	void RegisterInternalCalls();
	void RebuildClasses();

public:
	// Boxing
	float2 UnboxFloat2(MonoObject* obj);

	// Unboxing
	MonoObject* BoxFloat2(const float2& val);

public:
	ScriptingClass beengine_object_class;

	ScriptingClass float2_class;

	ScriptingClass game_object_class;

	ScriptingClass component_class;
	ScriptingClass component_script_class;
	ScriptingClass component_transform_class;

private:
	bool rebuild_internal_calls = true;
};

#endif // !__SCRIPTING_CLUSTER_H__