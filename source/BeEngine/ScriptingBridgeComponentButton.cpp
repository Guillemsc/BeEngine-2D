#include "ScriptingBridgeComponentButton.h"
#include "App.h"
#include "ModuleScripting.h"
#include "ScriptingCluster.h"
#include "ComponentButton.h"
#include "ModuleGameObject.h"
#include "ScriptingBridgeGameObject.h"
#include "ScriptingBridgeBeObject.h"

ScriptingBridgeComponentButton::ScriptingBridgeComponentButton(ComponentButton * component_button) 
	: ScriptingBridgeObject(App->scripting->scripting_cluster->component_button_class)
{
	component_button_ref = component_button;
}

ScriptingBridgeComponentButton::~ScriptingBridgeComponentButton()
{
}

void ScriptingBridgeComponentButton::Start()
{
}

void ScriptingBridgeComponentButton::OnRebuildInstances()
{
	if (class_instance != nullptr)
	{
		ScriptingBridgeBeObject::SetBeObjectRefPointer(class_instance->GetMonoObject(), component_button_ref);

		MonoObject* owner_go_mono_object = component_button_ref->GetOwner()->GetScriptingBridge()->GetInstance()->GetMonoObject();

		void* args2[1] = { owner_go_mono_object };

		MonoObject* ret_obj2 = nullptr;
		class_instance->InvokeMonoMethodOnParentClass(
			*App->scripting->scripting_cluster->component_class, "SetOwner", args2, 1, ret_obj2);
	}
}

void ScriptingBridgeComponentButton::CleanUp()
{
	if (class_instance != nullptr)
	{
		ScriptingBridgeBeObject::ClearBeObjectRefPointer(class_instance->GetMonoObject());
	}
}

void ScriptingBridgeComponentButton::CallOnClick()
{
	if (class_instance != nullptr)
	{
		MonoObject* ret_obj = nullptr;
		class_instance->InvokeMonoMethod("CallOnClick", nullptr, 0, ret_obj);
	}
}
