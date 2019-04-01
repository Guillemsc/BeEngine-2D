#ifndef __SCRIPTING_BRIDGE_COMPONENT_PHYSICS_BODY_H__
#define __SCRIPTING_BRIDGE_COMPONENT_PHYSICS_BODY_H__

#include "ScriptingBridgeObject.h"
#include "ModuleScripting.h"

class GameObject;
class GameObjectComponent;
class Event;
class ComponentPhysicsBody;

class ScriptingBridgeComponentPhysicsBody : public ScriptingBridgeObject
{
	friend class ScriptingCluster;
	friend class ModuleScripting;
	friend class ComponentPhysicsBody;

private:
	void operator delete(void *) {}

public:
	ScriptingBridgeComponentPhysicsBody(ComponentPhysicsBody* component_ref);
	~ScriptingBridgeComponentPhysicsBody();

	void Start();
	void OnRebuildInstances();
	void CleanUp();

private:
	static ComponentPhysicsBody* GetComponentPhysicsBodyFromMonoObject(MonoObject* mono_object);

	// Internal Calls


	// --------------

private:
	ComponentPhysicsBody* component_ref = nullptr;
};

#endif // !__SCRIPTING_BRIDGE_COMPONENT_PHYSICS_BODY_H__