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

	// Internal Calls

	static void SetText(MonoObject* mono_object, MonoString* mono_string);
	static MonoString* GetText(MonoObject* mono_object);

	// --------------

private:
	ComponentText* component_text_ref = nullptr;
};

#endif // !__SCRIPTING_BRIDGE_COMPONENT_TEXT_H__