#include "ScriptingBridgeComponentScript.h"
#include "ComponentScript.h"
#include "App.h"
#include "ModuleScripting.h"
#include "ScriptingCluster.h"
#include "GameObject.h"
#include "ScriptingBridgeGameObject.h"
#include "ComponentScriptFields.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

ScriptingBridgeComponentScript::ScriptingBridgeComponentScript(ComponentScript * component_script)
	: ScriptingBridgeObject(nullptr)
{
	component_script_ref = component_script;
}

ScriptingBridgeComponentScript::~ScriptingBridgeComponentScript()
{
}

void ScriptingBridgeComponentScript::Start()
{
}

void ScriptingBridgeComponentScript::OnRebuildInstances()
{
	if (class_generated.GetLoaded())
	{
		if (App->scripting->user_code_assembly != nullptr && App->scripting->user_code_assembly->GetAssemblyLoaded())
		{
			App->scripting->user_code_assembly->GetClass(class_generated.GetNamespace().c_str(),
				class_generated.GetName().c_str(), class_generated);

			class_instance = class_generated.CreateInstance();

			if (class_instance != nullptr)
			{
				MonoArray* mono_pointer = App->scripting->BoxPointer(component_script_ref);

				void* args[1] = { mono_pointer };

				MonoObject* ret_obj = nullptr;
				class_instance->InvokeMonoMethodOnParentClass(
					*App->scripting->scripting_cluster->beengine_object_class, "SetPointerRef", args, 1, ret_obj);

				MonoObject* owner_go_mono_object = component_script_ref->GetOwner()->GetScriptingBridge()->GetInstance()->GetMonoObject();

				void* args2[1] = { owner_go_mono_object };

				MonoObject* ret_obj2 = nullptr;
				class_instance->InvokeMonoMethodOnParentClass(
					*App->scripting->scripting_cluster->component_class, "SetOwner", args2, 1, ret_obj2);
			}
		}
	}
}

void ScriptingBridgeComponentScript::CleanUp()
{

}

void ScriptingBridgeComponentScript::SetGeneratedClass(const ScriptingClass & g_class)
{
	class_generated = g_class;
}

void ScriptingBridgeComponentScript::RemoveGeneratedClass()
{
	class_generated = ScriptingClass();
}

void ScriptingBridgeComponentScript::SetField(ComponentScriptField * field)
{
	if (field != nullptr && class_instance != nullptr)
	{
		class_instance->SetFieldValue(field->GetName().c_str(), field->VoidValue());
	}
}

void ScriptingBridgeComponentScript::CallAwake()
{
	if (class_instance != nullptr)
	{
		MonoObject* ret_obj = nullptr;
		class_instance->InvokeMonoMethod("Awake", nullptr, 0, ret_obj);
	}
}

void ScriptingBridgeComponentScript::CallStart()
{
	if (class_instance != nullptr)
	{
		MonoObject* ret_obj = nullptr;
		class_instance->InvokeMonoMethod("Start", nullptr, 0, ret_obj);
	}
}

void ScriptingBridgeComponentScript::CallUpdate()
{
	if (class_instance != nullptr)
	{
		MonoObject* ret_obj = nullptr;
		class_instance->InvokeMonoMethod("Update", nullptr, 0, ret_obj);
	}
}

void ScriptingBridgeComponentScript::CallOnDestroy()
{
	if (class_instance != nullptr)
	{
		MonoObject* ret_obj = nullptr;
		class_instance->InvokeMonoMethod("OnDestroy", nullptr, 0, ret_obj);
	}
}
