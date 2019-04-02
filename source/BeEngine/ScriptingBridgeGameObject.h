#ifndef __SCRIPTING_BRIDGE_GAME_OBJECT_H__
#define __SCRIPTING_BRIDGE_GAME_OBJECT_H__

#include "ScriptingBridgeObject.h"
#include "ModuleScripting.h"

class GameObject;
class GameObjectComponent;
class ComponentTransform;
class Event;
class PhysicsBody;

class ScriptingBridgeGameObject : public ScriptingBridgeObject
{
	friend class ScriptingCluster;
	friend class ModuleScripting;
	friend class GameObject;

private:
	void operator delete(void *) {}

public:
	ScriptingBridgeGameObject(GameObject* go_ref);
	~ScriptingBridgeGameObject();

	void Start();
	void OnRebuildInstances();
	void CleanUp();

	void SetComponentTransform(ComponentTransform* trans);

	void CallOnCollisionEnter(PhysicsBody* pb);
	void CallOnCollisionStay(PhysicsBody* pb);
	void CallOnCollisionExit(PhysicsBody* pb);

private:
	static GameObject* GetGameObjectFromMonoObject(MonoObject* mono_object);
	static GameObjectComponent* CreateGameObjectComponentFromComponentType(GameObject* go, MonoType* type);

	// Internal Calls

	static void SetName(MonoObject* mono_object, MonoString* mono_string);
	static MonoString* GetName(MonoObject * mono_object);

	static MonoObject* AddComponent(MonoObject* mono_object, MonoString* component_type);
	static MonoObject* GetComponent(MonoObject* mono_object, MonoString* component_type, int index);

	// --------------

private:
	GameObject* go_ref = nullptr;
};

#endif // !__SCRIPTING_BRIDGE_GAME_OBJECT_H__