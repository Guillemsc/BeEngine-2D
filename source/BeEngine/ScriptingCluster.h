#ifndef __SCRIPTING_CLUSTER_H__
#define __SCRIPTING_CLUSTER_H__

#include "ModuleScripting.h"

class PhysicsBody;

enum ScriptFieldType
{
	SCRIPT_FIELD_UNDEFINED,
	SCRIPT_FIELD_INT,
	SCRIPT_FIELD_FLOAT,
	SCRIPT_FIELD_STRING,
	SCRIPT_FIELD_BOOL,
	SCRIPT_FIELD_GAMEOBJECT,
};

class ScriptingCluster 
{
	friend class ModuleScripting;

private:
	void operator delete(void *) {}

private:
	void RegisterInternalCalls();
	void RebuildClasses();

public:
	ScriptFieldType GetScriptFieldTypeFromName(const std::string& name);

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
	ScriptingClass component_button_class;

	ScriptingClass collision_class;

private:
	bool rebuild_internal_calls = true;
};

#endif // !__SCRIPTING_CLUSTER_H__