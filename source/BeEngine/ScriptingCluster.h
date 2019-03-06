#ifndef __SCRIPTING_CLUSTER_H__
#define __SCRIPTING_CLUSTER_H__

#include "ModuleScripting.h"

class PhysicsBody;

class ScriptingCluster 
{
	friend class ModuleScripting;

private:
	void operator delete(void *) {}

private:
	void RegisterInternalCalls();
	void RebuildClasses();

public:
	// Unboxing
	float2 UnboxFloat2(MonoObject* obj);

	// Boxing
	MonoObject* BoxFloat2(const float2& val);

	MonoObject* BoxCollision(PhysicsBody* pb);

public:
	ScriptingClass beengine_object_class;

	ScriptingClass float2_class;

	ScriptingClass game_object_class;

	ScriptingClass component_class;
	ScriptingClass component_script_class;
	ScriptingClass component_transform_class;

	ScriptingClass collision_class;

private:
	bool rebuild_internal_calls = true;
};

#endif // !__SCRIPTING_CLUSTER_H__