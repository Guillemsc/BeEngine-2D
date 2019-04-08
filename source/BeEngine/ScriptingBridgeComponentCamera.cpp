#include "ScriptingBridgeComponentCamera.h"
#include "App.h"
#include "ComponentCamera.h"
#include "ModuleScripting.h"
#include "ScriptingCluster.h"
#include "GameObject.h"

ScriptingBridgeComponentCamera::ScriptingBridgeComponentCamera(ComponentCamera * component) :
	ScriptingBridgeObject(App->scripting->scripting_cluster->component_camera_class)
{
	component_ref = component;
}

ScriptingBridgeComponentCamera::~ScriptingBridgeComponentCamera()
{
}

void ScriptingBridgeComponentCamera::Start()
{
}

void ScriptingBridgeComponentCamera::OnRebuildInstances()
{
	if (class_instance != nullptr)
	{
		MonoString* mono_pointer = App->scripting->BoxPointer(component_ref);

		void* args[1] = { mono_pointer };

		MonoObject* ret_obj = nullptr;
		class_instance->InvokeMonoMethodOnParentClass(
			*App->scripting->scripting_cluster->beengine_object_class, "SetPointerRef", args, 1, ret_obj);

		MonoObject* owner_go_mono_object = component_ref->GetOwner()->GetScriptingBridge()->GetInstance()->GetMonoObject();

		void* args2[1] = { owner_go_mono_object };

		MonoObject* ret_obj2 = nullptr;
		class_instance->InvokeMonoMethodOnParentClass(
			*App->scripting->scripting_cluster->component_class, "SetOwner", args2, 1, ret_obj2);
	}
}

void ScriptingBridgeComponentCamera::CleanUp()
{
}

ComponentCamera * ScriptingBridgeComponentCamera::GetComponentCameraFromMonoObject(MonoObject * mono_object)
{
	return nullptr;
}
