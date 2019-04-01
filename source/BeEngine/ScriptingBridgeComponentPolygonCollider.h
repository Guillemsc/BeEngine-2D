#ifndef __SCRIPTING_BRIDGE_COMPONENT_POLYGON_COLLIDER_H__
#define __SCRIPTING_BRIDGE_COMPONENT_POLYGON_COLLIDER_H__

#include "ScriptingBridgeObject.h"
#include "ModuleScripting.h"

class GameObject;
class GameObjectComponent;
class Event;
class ComponentPolygonCollider;

class ScriptingBridgeComponentPolygonCollider : public ScriptingBridgeObject
{
	friend class ScriptingCluster;
	friend class ModuleScripting;
	friend class ComponentPolygonCollider;

private:
	void operator delete(void *) {}

public:
	ScriptingBridgeComponentPolygonCollider(ComponentPolygonCollider* component_ref);
	~ScriptingBridgeComponentPolygonCollider();

	void Start();
	void OnRebuildInstances();
	void CleanUp();

private:
	static ComponentPolygonCollider* GetComponentPolygonColliderFromMonoObject(MonoObject* mono_object);

	// Internal Calls


	// --------------

private:
	ComponentPolygonCollider* component_ref = nullptr;
};

#endif // !__SCRIPTING_BRIDGE_COMPONENT_POLYGON_COLLIDER_H__