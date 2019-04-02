#ifndef __SCRIPTING_BRIDGE_INPUT_H__
#define __SCRIPTING_BRIDGE_INPUT_H__

#include "ScriptingBridgeObject.h"
#include "ModuleScripting.h"

class GameObject;
class GameObjectComponent;
class ComponentTransform;
class Event;
class PhysicsBody;

class ScriptingBridgeInput : public ScriptingBridgeObject
{
	friend class ScriptingCluster;
	friend class ModuleScripting;
	friend class GameObject;

private:
	void operator delete(void *) {}

public:
	ScriptingBridgeInput();
	~ScriptingBridgeInput();

	void Start();
	void RebuildInstances();
	void PostRebuildInstances();
	void CleanUp();

private:

	// Internal Calls

	static bool GetKeyDown(MonoString* mono_string);
	static bool GetKeyRepeat(MonoString* mono_string);
	static bool GetKeyUp(MonoString* mono_string);

	// --------------
};

#endif // !__SCRIPTING_BRIDGE_INPUT_H__