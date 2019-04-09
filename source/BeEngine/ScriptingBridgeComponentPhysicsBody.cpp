#include "ScriptingBridgeComponentPhysicsBody.h"
#include "App.h"
#include "ComponentPhysicsBody.h"
#include "ModuleScripting.h"
#include "ScriptingCluster.h"
#include "GameObject.h"
#include "ScriptingBridgeBeObject.h"

ScriptingBridgeComponentPhysicsBody::ScriptingBridgeComponentPhysicsBody(ComponentPhysicsBody * component) :
	ScriptingBridgeObject(App->scripting->scripting_cluster->component_physics_body_class)
{
	component_ref = component;
}

ScriptingBridgeComponentPhysicsBody::~ScriptingBridgeComponentPhysicsBody()
{
}

void ScriptingBridgeComponentPhysicsBody::Start()
{
}

void ScriptingBridgeComponentPhysicsBody::OnRebuildInstances()
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

void ScriptingBridgeComponentPhysicsBody::CleanUp()
{
	if (class_instance != nullptr)
	{
		ScriptingBridgeBeObject::ClearBeObjectRefPointer(class_instance->GetMonoObject());
	}
}

