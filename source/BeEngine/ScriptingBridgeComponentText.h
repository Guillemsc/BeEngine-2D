#ifndef __SCRIPTING_BRIDGE_COMPONENT_TEXT_H__
#define __SCRIPTING_BRIDGE_COMPONENT_TEXT_H__

#include "ScriptingBridgeObject.h"
#include "ModuleScripting.h"

class GameObject;
class GameObjectComponent;
class ComponentText;
class Event;

class ScriptingBridgeComponentText : public ScriptingBridgeObject
{
	friend class ScriptingCluster;
	friend class ModuleScripting;
	friend class ComponentText;

private:
	void operator delete(void *) {}

public:
	ScriptingBridgeComponentText(ComponentText* component_text_ref);
	~ScriptingBridgeComponentText();

	void Start();
	void OnRebuildInstances();
	void CleanUp();

private:
	static ComponentText* GetComponentButtonFromMonoObject(MonoObject* mono_object);
	//static GameObjectComponent* CreateGameObjectComponentFromComponentType(GameObject* go, MonoType* type);

	// Internal Calls

	//static void AddComponent(MonoObject* mono_object, MonoType* component_type);

	// --------------

private:
	ComponentText* component_text_ref = nullptr;
};

#endif // !__SCRIPTING_BRIDGE_COMPONENT_TEXT_H__