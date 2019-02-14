#ifndef __SCRIPTING_ITEM_GAME_OBJECT_H__
#define __SCRIPTING_ITEM_GAME_OBJECT_H__

#include "ScriptingItem.h"
#include "ModuleScripting.h"

class GameObject;
class GameObjectComponent;
class Event;

class ScriptingItemGameObject : public ScriptingItem
{
	friend class ModuleScripting;

private:
	void operator delete(void *) {}

public:
	ScriptingItemGameObject();
	~ScriptingItemGameObject();

	void Start();
	void RegisterInternalCalls();
	void CleanUp();

	void OnEvent(Event* ev);

	void RebuildClasses();

	void RebuildInstances();
	void DestroyInstances();

private:
	void AddScriptingInstance(GameObject* go);
	void RemoveScriptingInstance(GameObject* go);

	static GameObject* GetGameObjectFromMonoObject(MonoObject* mono_object);
	static GameObjectComponent* CreateGameObjectComponentFromComponentType(GameObject* go, MonoType* type);

	// Internal Calls

	static void SetName(MonoObject* mono_object, MonoString* mono_string);
	static MonoString* GetName(MonoObject * mono_object);

	static void AddComponent(MonoObject* mono_object, MonoType* component_type);

	// --------------

private:
	ScriptingClass game_object_class;
	ScriptingClass beengine_object_class;

};

#endif // !__SCRIPTING_ITEM_GAME_OBJECT_H__