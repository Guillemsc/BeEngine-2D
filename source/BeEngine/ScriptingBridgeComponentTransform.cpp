#include "ScriptingBridgeComponentTransform.h"
#include "App.h"
#include "ScriptingCluster.h"
#include "GameObject.h"
#include "ScriptingBridgeGameObject.h"
#include "ComponentTransfrom.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

ScriptingBridgeComponentTransform::ScriptingBridgeComponentTransform(ComponentTransform * component_transform)
	: ScriptingBridgeObject(App->scripting->scripting_cluster->component_transform_class)
{
	component_transform_ref = component_transform;
}

ScriptingBridgeComponentTransform::~ScriptingBridgeComponentTransform()
{
}

void ScriptingBridgeComponentTransform::Start()
{
}

void ScriptingBridgeComponentTransform::OnRebuildInstances()
{
	if (class_instance != nullptr)
	{
		MonoArray* mono_pointer = App->scripting->BoxPointer(component_transform_ref);

		void* args[1] = { mono_pointer };

		MonoObject* ret_obj = nullptr;
		class_instance->InvokeMonoMethodOnParentClass(
			*App->scripting->scripting_cluster->beengine_object_class, "SetPointerRef", args, 1, ret_obj);

		MonoObject* owner_go_mono_object = component_transform_ref->GetOwner()->GetScriptingBridge()->GetInstance()->GetMonoObject();

		void* args2[1] = { owner_go_mono_object };

		MonoObject* ret_obj2 = nullptr;
		class_instance->InvokeMonoMethodOnParentClass(
			*App->scripting->scripting_cluster->component_class, "SetOwner", args2, 1, ret_obj2);

		component_transform_ref->GetOwner()->GetScriptingBridge()->SetComponentTransform(component_transform_ref);
	}
}

void ScriptingBridgeComponentTransform::CleanUp()
{
}

ComponentTransform * ScriptingBridgeComponentTransform::GetComponentTransformFromMonoObject(MonoObject * mono_object)
{
	ComponentTransform* ret = nullptr;

	if (mono_object != nullptr)
	{
		MonoObject* obj_ret = nullptr;
		if (App->scripting->InvokeMonoMethod(mono_object,
			App->scripting->scripting_cluster->beengine_object_class->GetMonoClass(), "GetPointerRef", nullptr, 0, obj_ret))
		{
			if (obj_ret != nullptr)
			{
				ret = (ComponentTransform*)App->scripting->UnboxPointer((MonoArray*)obj_ret);
			}
		}
	}

	return ret;
}

void ScriptingBridgeComponentTransform::SetPosition(MonoObject * mono_object, MonoObject * mono_float2)
{
	ComponentTransform* component_trans = GetComponentTransformFromMonoObject(mono_object);

	if (component_trans != nullptr)
	{
		float2 new_pos = App->scripting->scripting_cluster->UnboxFloat2(mono_float2);

		component_trans->SetPosition(new_pos);
	}
}

MonoObject * ScriptingBridgeComponentTransform::GetPosition(MonoObject * mono_object)
{
	return nullptr;
}
