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
	void RebuildInstances();
	void PostRebuildInstances();
	void CleanUp();

	ScriptingClassInstance* GetComponentTransformScriptingInstance() const;

private:
	static ComponentTransform* GetComponentTransformFromMonoObject(MonoObject* mono_object);
	//static GameObjectComponent* CreateGameObjectComponentFromComponentType(GameObject* go, MonoType* type);

	// Internal Calls

	static void SetPosition(MonoObject* mono_object, MonoObject* mono_float2);
	static MonoObject* GetPosition(MonoObject * mono_object);

	//static void AddComponent(MonoObject* mono_object, MonoType* component_type);

	// --------------

private:
	ComponentTransform* component_transform_ref = nullptr;
	ScriptingClass class_generated;
	ScriptingClassInstance* component_transform_scripting_instance = nullptr;
};

#endif // !__SCRIPTING_BRIDGE_COMPONENT_TRANSFORM_H__