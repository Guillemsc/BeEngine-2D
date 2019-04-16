#include "ScriptingBridgeComponentCamera.h"
#include "App.h"
#include "ComponentCamera.h"
#include "ModuleScripting.h"
#include "ScriptingCluster.h"
#include "GameObject.h"
#include "ScriptingBridgeBeObject.h"

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
		ScriptingBridgeBeObject::SetBeObjectRefPointer(class_instance->GetMonoObject(), component_ref);

		MonoObject* owner_go_mono_object = component_ref->GetOwner()->GetScriptingBridge()->GetInstance()->GetMonoObject();

		void* args2[1] = { owner_go_mono_object };

		MonoObject* ret_obj2 = nullptr;
		class_instance->InvokeMonoMethodOnParentClass(
			*App->scripting->scripting_cluster->component_class, "SetOwner", args2, 1, ret_obj2);
	}
}

void ScriptingBridgeComponentCamera::CleanUp()
{
	if (class_instance != nullptr)
	{
		ScriptingBridgeBeObject::ClearBeObjectRefPointer(class_instance->GetMonoObject());
	}
}

float ScriptingBridgeComponentCamera::GetViewUpPosition(MonoObject * mono_object)
{
	float ret = 0.0f;

	ComponentCamera* comp = (ComponentCamera*)ScriptingBridgeBeObject::GetBeObjectRefPointer(mono_object);

	if (comp != nullptr)
	{
		ret = comp->GetViewUpPosition();
	}

	return ret;
}

float ScriptingBridgeComponentCamera::GetViewDownPosition(MonoObject * mono_object)
{
	float ret = 0.0f;

	ComponentCamera* comp = (ComponentCamera*)ScriptingBridgeBeObject::GetBeObjectRefPointer(mono_object);

	if (comp != nullptr)
	{
		ret = comp->GetViewDownPosition();
	}

	return ret;
}

float ScriptingBridgeComponentCamera::GetViewLeftPosition(MonoObject * mono_object)
{
	float ret = 0.0f;

	ComponentCamera* comp = (ComponentCamera*)ScriptingBridgeBeObject::GetBeObjectRefPointer(mono_object);

	if (comp != nullptr)
	{
		ret = comp->GetViewLeftPosition();
	}

	return ret;
}

float ScriptingBridgeComponentCamera::GetViewRightPosition(MonoObject * mono_object)
{
	float ret = 0.0f;

	ComponentCamera* comp = (ComponentCamera*)ScriptingBridgeBeObject::GetBeObjectRefPointer(mono_object);

	if (comp != nullptr)
	{
		ret = comp->GetViewRightPosition();
	}

	return ret;
}
