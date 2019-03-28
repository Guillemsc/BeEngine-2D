#ifndef __SCRIPTING_BRIDGE_COMPONENT_SCRIPT_H__
#define __SCRIPTING_BRIDGE_COMPONENT_SCRIPT_H__

#include "ScriptingBridgeObject.h"
#include "ModuleScripting.h"

class GameObject;
class GameObjectComponent;
class Event;

class ScriptingBridgeComponentScript : public ScriptingBridgeObject
{
	friend class ScriptingCluster;
	friend class ModuleScripting;
	friend class ComponentScript;

private:
	void operator delete(void *) {}

public:
	ScriptingBridgeComponentScript(ComponentScript* component_script_ref);
	~ScriptingBridgeComponentScript();

	void Start();
	void RebuildInstances();
	void PostRebuildInstances();
	void CleanUp();

	ScriptingClassInstance* GetComponentScriptScriptingInstance() const;

	void SetGeneratedClass(const ScriptingClass& generated_class);
	void RemoveGeneratedClass();

	void SetField(ComponentScriptField* field);

	void CallAwake();
	void CallStart();
	void CallUpdate();
	void CallOnDestroy();

private:
	//static GameObject* GetGameObjectFromMonoObject(MonoObject* mono_object);
	//static GameObjectComponent* CreateGameObjectComponentFromComponentType(GameObject* go, MonoType* type);

	// Internal Calls

	//static void SetName(MonoObject* mono_object, MonoString* mono_string);
	//static MonoString* GetName(MonoObject * mono_object);

	//static void AddComponent(MonoObject* mono_object, MonoType* component_type);

	// --------------

private:
	ComponentScript* component_script_ref = nullptr;
	ScriptingClass class_generated;
	ScriptingClassInstance* component_script_scripting_instance = nullptr;
};

#endif // !__SCRIPTING_BRIDGE_COMPONENT_SCRIPT_H__