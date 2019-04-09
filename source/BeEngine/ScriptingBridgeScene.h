#ifndef __SCRIPTING_BRIDGE_SCENE_H__
#define __SCRIPTING_BRIDGE_SCENE_H__

#include "ScriptingBridgeObject.h"
#include "ModuleScripting.h"

class GameObject;
class GameObjectComponent;
class ComponentTransform;
class Event;
class PhysicsBody;

class ScriptingBridgeScene : public ScriptingBridgeObject
{
	friend class ScriptingCluster;
	friend class ModuleScripting;
	friend class GameObject;

private:
	void operator delete(void *) {}

public:
	ScriptingBridgeScene();
	~ScriptingBridgeScene();

	void Start();
	void RebuildInstances();
	void PostRebuildInstances();
	void CleanUp();

private:

	// Internal Calls

	static MonoObject* CreateGameObject();
	static void DestroyGameObject(MonoObject* mono_object);
	static void DestroyComponent(MonoObject* mono_object);
	static void LoadScene(MonoString* mono_string);

	// --------------
};

#endif // !__SCRIPTING_BRIDGE_SCENE_H__