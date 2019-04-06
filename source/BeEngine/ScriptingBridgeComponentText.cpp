#include "ScriptingBridgeComponentText.h"
#include "App.h"
#include "ModuleScripting.h"
#include "ScriptingCluster.h"
#include "ComponentText.h"
#include "GameObject.h"

ScriptingBridgeComponentText::ScriptingBridgeComponentText(ComponentText * component_text) :
	ScriptingBridgeObject(App->scripting->scripting_cluster->component_button_class)
{
	component_text_ref = component_text;
}

ScriptingBridgeComponentText::~ScriptingBridgeComponentText()
{
}

void ScriptingBridgeComponentText::Start()
{
}

void ScriptingBridgeComponentText::OnRebuildInstances()
{
	if (class_instance != nullptr)
	{
		MonoArray* mono_pointer = App->scripting->BoxPointer(component_text_ref);

		void* args[1] = { mono_pointer };

		MonoObject* ret_obj = nullptr;
		class_instance->InvokeMonoMethodOnParentClass(
			*App->scripting->scripting_cluster->beengine_object_class, "SetPointerRef", args, 1, ret_obj);

		MonoObject* owner_go_mono_object = component_text_ref->GetOwner()->GetScriptingBridge()->GetInstance()->GetMonoObject();

		void* args2[1] = { owner_go_mono_object };

		MonoObject* ret_obj2 = nullptr;
		class_instance->InvokeMonoMethodOnParentClass(
			*App->scripting->scripting_cluster->component_class, "SetOwner", args2, 1, ret_obj2);
	}
}

void ScriptingBridgeComponentText::CleanUp()
{
}

ComponentText * ScriptingBridgeComponentText::GetComponentButtonFromMonoObject(MonoObject * mono_object)
{
	return nullptr;
}
