#include "ScriptingBridgeComponentButton.h"
#include "App.h"
#include "ModuleScripting.h"
#include "ScriptingCluster.h"
#include "ComponentButton.h"
#include "ModuleGameObject.h"
#include "ScriptingBridgeGameObject.h"

ScriptingBridgeComponentButton::ScriptingBridgeComponentButton(ComponentButton * component_button)
{
	component_button_ref = component_button;
}

ScriptingBridgeComponentButton::~ScriptingBridgeComponentButton()
{
}

void ScriptingBridgeComponentButton::Start()
{
}

void ScriptingBridgeComponentButton::RebuildInstances()
{
	if (component_button_scripting_instance != nullptr)
	{
		component_button_scripting_instance->DestroyReference();
		RELEASE(component_button_scripting_instance);
	}

	component_button_scripting_instance = App->scripting->scripting_cluster->component_button_class.CreateInstance();

	if (component_button_scripting_instance != nullptr)
	{
		MonoArray* mono_pointer = App->scripting->BoxPointer(component_button_ref);

		void* args[1] = { mono_pointer };

		MonoObject* ret_obj = nullptr;
		component_button_scripting_instance->InvokeMonoMethodOnParentClass(
			App->scripting->scripting_cluster->beengine_object_class, "SetPointerRef", args, 1, ret_obj);
	}
}

void ScriptingBridgeComponentButton::PostRebuildInstances()
{
	if (component_button_scripting_instance != nullptr)
	{
		MonoObject* owner_go_mono_object = component_button_ref->GetOwner()->GetScriptingBridge()->GetGoScriptingInstance()->GetMonoObject();

		void* args[1] = { owner_go_mono_object };

		MonoObject* ret_obj = nullptr;
		component_button_scripting_instance->InvokeMonoMethodOnParentClass(
			App->scripting->scripting_cluster->component_class, "SetOwner", args, 1, ret_obj);
	}
}

void ScriptingBridgeComponentButton::CleanUp()
{
}

ScriptingClassInstance * ScriptingBridgeComponentButton::GetComponentButtonScriptingInstance() const
{
	return component_button_scripting_instance;
}

void ScriptingBridgeComponentButton::CallOnClick()
{
	if (component_button_scripting_instance != nullptr)
	{
		MonoObject* ret_obj = nullptr;
		component_button_scripting_instance->InvokeMonoMethod("CallOnClick", nullptr, 0, ret_obj);
	}
}

ComponentButton * ScriptingBridgeComponentButton::GetComponentButtonFromMonoObject(MonoObject * mono_object)
{
	ComponentButton* ret = nullptr;

	if (mono_object != nullptr)
	{
		MonoObject* obj_ret = nullptr;
		if (App->scripting->InvokeMonoMethod(mono_object,
			App->scripting->scripting_cluster->beengine_object_class.GetMonoClass(), "GetPointerRef", nullptr, 0, obj_ret))
		{
			if (obj_ret != nullptr)
			{
				ret = (ComponentButton*)App->scripting->UnboxPointer((MonoArray*)obj_ret);
			}
		}
	}

	return ret;
}
