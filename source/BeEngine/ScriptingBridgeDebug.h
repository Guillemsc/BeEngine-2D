#ifndef __SCRIPTING_BRIDGE_DEBUG_H__
#define __SCRIPTING_BRIDGE_DEBUG_H__

#include "ScriptingBridgeObject.h"
#include "ModuleScripting.h"

class GameObject;
class GameObjectComponent;
class ComponentTransform;
class Event;
class PhysicsBody;

class ScriptingBridgeDebug : public ScriptingBridgeObject
{
	friend class ScriptingCluster;
	friend class ModuleScripting;
	friend class GameObject;

private:
	void operator delete(void *) {}

public:
	ScriptingBridgeDebug();
	~ScriptingBridgeDebug();

	void Start();
	void RebuildInstances();
	void PostRebuildInstances();
	void CleanUp();

private:

	// Internal Calls

	static void Log(MonoString* mono_string);

	// --------------
};

#endif // !__SCRIPTING_BRIDGE_DEBUG_H__