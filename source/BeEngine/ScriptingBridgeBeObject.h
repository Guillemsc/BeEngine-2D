#ifndef __SCRIPTING_BRIDGE_BEOBJECT_H__
#define __SCRIPTING_BRIDGE_BEOBJECT_H__

#include "ScriptingBridgeObject.h"
#include "ModuleScripting.h"

class GameObject;
class GameObjectComponent;
class ComponentTransform;
class Event;
class PhysicsBody;

class ScriptingBridgeBeObject : public ScriptingBridgeObject
{
	friend class ScriptingCluster;
	friend class ModuleScripting;
	friend class GameObject;

private:
	void operator delete(void *) {}

public:
	ScriptingBridgeBeObject();
	~ScriptingBridgeBeObject();

	void Start();
	void RebuildInstances();
	void PostRebuildInstances();
	void CleanUp();

	static bool SetBeObjectRefPointer(MonoObject* obj, void* pointer);
	static void* GetBeObjectRefPointer(MonoObject* obj);
	static void ClearBeObjectRefPointer(MonoObject* obj);
	static bool GetBeObjectRefValid(MonoObject* obj);

private:

};

#endif // !__SCRIPTING_BRIDGE_BEOBJECT_H__