#include "ScriptingBridgeComponentScript.h"
#include "ComponentScript.h"
#include "App.h"
#include "ModuleScripting.h"
#include "ScriptingCluster.h"
#include "GameObject.h"
#include "ScriptingBridgeGameObject.h"

ScriptingBridgeComponentScript::ScriptingBridgeComponentScript(ComponentScript * component_script)
{
	component_script_ref = component_script;
}

ScriptingBridgeComponentScript::~ScriptingBridgeComponentScript()
{
}

void ScriptingBridgeComponentScript::Start()
{
}

void ScriptingBridgeComponentScript::RebuildInstances()
{
	if (component_script_scripting_instance != nullptr)
	{
		component_script_scripting_instance->CleanUp();
		RELEASE(component_script_scripting_instance);
	}

	if (class_generated.GetLoaded())
	{
		component_script_scripting_instance = class_generated.CreateInstance();

		if (component_script_scripting_instance != nullptr)
		{
			MonoArray* mono_pointer = App->scripting->BoxPointer(component_script_ref);

			void* args[1] = { mono_pointer };

			MonoObject* ret_obj = nullptr;
			component_script_scripting_instance->InvokeMonoMethodOnParentClass(
				App->scripting->scripting_cluster->beengine_object_class, "SetPointerRef", args, 1, ret_obj);
		}
	}
}

void ScriptingBridgeComponentScript::PostRebuildInstances()
{
	if (component_script_scripting_instance != nullptr)
	{
		MonoObject* owner_go_mono_object = component_script_ref->GetOwner()->GetScriptingBridge()->GetGoScriptingInstance()->GetMonoObject();

		void* args[1] = { owner_go_mono_object };

		MonoObject* ret_obj = nullptr;
		component_script_scripting_instance->InvokeMonoMethodOnParentClass(
			App->scripting->scripting_cluster->component_class, "SetOwner", args, 1, ret_obj);
	}
}

void ScriptingBridgeComponentScript::CleanUp()
{
	if (component_script_scripting_instance != nullptr)
	{
		component_script_scripting_instance->CleanUp();
		RELEASE(component_script_scripting_instance);
	}
}

void ScriptingBridgeComponentScript::SetGeneratedClass(const ScriptingClass & g_class)
{
	class_generated = g_class;
}

void ScriptingBridgeComponentScript::RemoveGeneratedClass()
{
	class_generated = ScriptingClass();
}

void ScriptingBridgeComponentScript::CallAwake()
{
	if (component_script_scripting_instance != nullptr)
	{
		MonoObject* ret_obj = nullptr;
		component_script_scripting_instance->InvokeMonoMethod("Awake", nullptr, 0, ret_obj);
	}
}

void ScriptingBridgeComponentScript::CallStart()
{
	if (component_script_scripting_instance != nullptr)
	{
		MonoObject* ret_obj = nullptr;
		component_script_scripting_instance->InvokeMonoMethod("Start", nullptr, 0, ret_obj);
	}
}

void ScriptingBridgeComponentScript::CallUpdate()
{
	if (component_script_scripting_instance != nullptr)
	{
		MonoObject* ret_obj = nullptr;
		component_script_scripting_instance->InvokeMonoMethod("Update", nullptr, 0, ret_obj);
	}
}

void ScriptingBridgeComponentScript::CallOnDestroy()
{
	if (component_script_scripting_instance != nullptr)
	{
		MonoObject* ret_obj = nullptr;
		component_script_scripting_instance->InvokeMonoMethod("OnDestroy", nullptr, 0, ret_obj);
	}
}
