#ifndef __SCRIPTING_BRIDGE_TIME_H__
#define __SCRIPTING_BRIDGE_TIME_H__

#include "ScriptingBridgeObject.h"
#include "ModuleScripting.h"

class GameObject;
class GameObjectComponent;
class ComponentTransform;
class Event;
class PhysicsBody;

class ScriptingBridgeTime : public ScriptingBridgeObject
{
	friend class ScriptingCluster;
	friend class ModuleScripting;
	friend class GameObject;

private:
	void operator delete(void *) {}

public:
	ScriptingBridgeTime();
	~ScriptingBridgeTime();

	void Start();
	void RebuildInstances();
	void PostRebuildInstances();
	void CleanUp();

private:

	// Internal Calls

	static float GetDeltaTime();

	// --------------
};

#endif // !__SCRIPTING_BRIDGE_TIME_H__