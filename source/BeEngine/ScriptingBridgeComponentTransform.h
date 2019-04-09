#ifndef __SCRIPTING_BRIDGE_COMPONENT_TRANSFORM_H__
#define __SCRIPTING_BRIDGE_COMPONENT_TRANSFORM_H__

#include "ScriptingBridgeObject.h"
#include "ModuleScripting.h"

class GameObject;
class GameObjectComponent;
class Event;

class ScriptingBridgeComponentTransform : public ScriptingBridgeObject
{
	friend class ScriptingCluster;
	friend class ModuleScripting;
	friend class ComponentTransform;

private:
	void operator delete(void *) {}

public:
	ScriptingBridgeComponentTransform(ComponentTransform* component_transform_ref);
	~ScriptingBridgeComponentTransform();

	void Start();
	void OnRebuildInstances();
	void CleanUp();

private:
	// Internal Calls

	static void SetPosition(MonoObject* mono_object, MonoObject* mono_float2);
	static MonoObject* GetPosition(MonoObject * mono_object);

	static void SetRotationDegrees(MonoObject* mono_object, float rotation);
	static float GetRotationDegrees(MonoObject * mono_object);

	static void SetScale(MonoObject* mono_object, MonoObject* mono_float2);
	static MonoObject* GetScale(MonoObject * mono_object);

	// --------------

private:
	ComponentTransform* component_transform_ref = nullptr;
};

#endif // !__SCRIPTING_BRIDGE_COMPONENT_TRANSFORM_H__