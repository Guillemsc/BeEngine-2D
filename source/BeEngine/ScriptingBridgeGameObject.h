#ifndef __SCRIPTING_BRIDGE_GAME_OBJECT_H__
#define __SCRIPTING_BRIDGE_GAME_OBJECT_H__

#include "ScriptingBridgeObject.h"
#include "ModuleScripting.h"

class GameObject;
class GameObjectComponent;
class ComponentTransform;
class Event;

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
	void RebuildInstances();
	void PostRebuildInstances();
	void CleanUp();

	ScriptingClassInstance* GetGoScriptingInstance() const;

	void SetComponentTransform(ComponentTransform* trans);

private:
	static GameObject* GetGameObjectFromMonoObject(MonoObject* mono_object);
	static GameObjectComponent* CreateGameObjectComponentFromComponentType(GameObject* go, MonoType* type);

	// Internal Calls

	static void SetName(MonoObject* mono_object, MonoString* mono_string);
	static MonoString* GetName(MonoObject * mono_object);

	static void AddComponent(MonoObject* mono_object, MonoType* component_type);

	// --------------

private:
	GameObject* go_ref = nullptr;
	ScriptingClassInstance* go_scripting_instance = nullptr;
};

#endif // !__SCRIPTING_BRIDGE_GAME_OBJECT_H__