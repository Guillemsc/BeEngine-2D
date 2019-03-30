#include "ScriptingBridgeComponentButton.h"
#include "App.h"
#include "ModuleScripting.h"
#include "ScriptingCluster.h"
#include "ComponentButton.h"
#include "ModuleGameObject.h"
#include "ScriptingBridgeGameObject.h"

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
		MonoArray* mono_pointer = App->scripting->BoxPointer(component_button_ref);

		void* args[1] = { mono_pointer };

		MonoObject* ret_obj = nullptr;
		class_instance->InvokeMonoMethodOnParentClass(
			*App->scripting->scripting_cluster->beengine_object_class, "SetPointerRef", args, 1, ret_obj);

		MonoObject* owner_go_mono_object = component_button_ref->GetOwner()->GetScriptingBridge()->GetInstance()->GetMonoObject();

		void* args2[1] = { owner_go_mono_object };

		MonoObject* ret_obj2 = nullptr;
		class_instance->InvokeMonoMethodOnParentClass(
			*App->scripting->scripting_cluster->component_class, "SetOwner", args2, 1, ret_obj2);
	}
}

void ScriptingBridgeComponentButton::CleanUp()
{
}

void ScriptingBridgeComponentButton::CallOnClick()
{
	if (class_instance != nullptr)
	{
		MonoObject* ret_obj = nullptr;
		class_instance->InvokeMonoMethod("CallOnClick", nullptr, 0, ret_obj);
	}
}

ComponentButton * ScriptingBridgeComponentButton::GetComponentButtonFromMonoObject(MonoObject * mono_object)
{
	ComponentButton* ret = nullptr;

	if (mono_object != nullptr)
	{
		MonoObject* obj_ret = nullptr;
		if (App->scripting->InvokeMonoMethod(mono_object,
			App->scripting->scripting_cluster->beengine_object_class->GetMonoClass(), "GetPointerRef", nullptr, 0, obj_ret))
		{
			if (obj_ret != nullptr)
			{
				ret = (ComponentButton*)App->scripting->UnboxPointer((MonoArray*)obj_ret);
			}
		}
	}

	return ret;
}
