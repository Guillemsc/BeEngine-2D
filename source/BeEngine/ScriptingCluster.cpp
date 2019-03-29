#include "ScriptingCluster.h"
#include "App.h"
#include "ModuleScripting.h"
#include "ScriptingBridgeGameObject.h"
#include "ScriptingBridgeComponentTransform.h"
#include "ScriptingBridgeDebug.h"
#include "PhysicsBody.h"
#include "ComponentTransfrom.h"
#include "GameObject.h"

void ScriptingCluster::RegisterInternalCalls()
{
	if (rebuild_internal_calls)
	{
		rebuild_internal_calls = false;

		// Debug ------------------------------------
		mono_add_internal_call("BeEngine.Debug::Log", (const void*)ScriptingBridgeDebug::Log);
		// ------------------------------------ Debug

		// GameObject -------------------------------
		mono_add_internal_call("BeEngine.GameObject::SetName", (const void*)ScriptingBridgeGameObject::SetName);
		mono_add_internal_call("BeEngine.GameObject::GetName", (const void*)ScriptingBridgeGameObject::GetName);
		mono_add_internal_call("BeEngine.GameObject::AddComponentTypeName", (const void*)ScriptingBridgeGameObject::AddComponent);
		// ------------------------------- GameObject

		// ComponentTransform -----------------------
		mono_add_internal_call("BeEngine.ComponentTransform::SetPosition", (const void*)ScriptingBridgeComponentTransform::SetPosition);
		mono_add_internal_call("BeEngine.ComponentTransform::GetPosition", (const void*)ScriptingBridgeComponentTransform::GetPosition);
		// ----------------------- ComponentTransform

	}
}

void ScriptingCluster::RebuildClasses()
{
	if (App->scripting->scripting_assembly != nullptr && App->scripting->scripting_assembly->GetAssemblyLoaded())
	{
		// BeEngineObject
		App->scripting->scripting_assembly->GetClass("BeEngine", "BeEngineObject", beengine_object_class);

		// Debug
		App->scripting->scripting_assembly->GetClass("BeEngine", "Debug", debug_class);

		// Math
		App->scripting->scripting_assembly->GetClass("BeEngine", "float2", float2_class);

		// GameObject
		App->scripting->scripting_assembly->GetClass("BeEngine", "GameObject", game_object_class);

		// Components
		App->scripting->scripting_assembly->GetClass("BeEngine", "Component", component_class);

		App->scripting->scripting_assembly->GetClass("BeEngine", "ComponentScript", component_script_class);
		App->scripting->scripting_assembly->GetClass("BeEngine", "ComponentTransform", component_transform_class);
		App->scripting->scripting_assembly->GetClass("BeEngine", "ComponentButton", component_button_class);

		// Physics
		App->scripting->scripting_assembly->GetClass("BeEngine", "Collision", collision_class);
	}
}

ScriptFieldType ScriptingCluster::GetScriptFieldTypeFromName(const std::string & name)
{
	ScriptFieldType ret = ScriptFieldType::SCRIPT_FIELD_UNDEFINED;

	if (name.compare("System.String") == 0)
	{
		ret = ScriptFieldType::SCRIPT_FIELD_STRING;
	}
	else if (name.compare("System.Int32") == 0)
	{
		ret = ScriptFieldType::SCRIPT_FIELD_INT;
	}
	else if (name.compare("System.Single") == 0)
	{
		ret = ScriptFieldType::SCRIPT_FIELD_FLOAT;
	}
	else if (name.compare("System.Boolean") == 0)
	{
		ret = ScriptFieldType::SCRIPT_FIELD_BOOL;
	}
	else if (name.compare("BeEngine.GameObject") == 0)
	{
		ret = ScriptFieldType::SCRIPT_FIELD_GAMEOBJECT;
	}

	return ret;
}

float2 ScriptingCluster::UnboxFloat2(MonoObject* obj)
{
	float2 ret = float2::zero;

	if (obj != nullptr)
	{
		MonoObject* x_val = App->scripting->GetFieldValue(obj, float2_class.GetMonoClass(), "_x");
		MonoObject* y_val = App->scripting->GetFieldValue(obj, float2_class.GetMonoClass(), "_y");

		if (x_val != nullptr && y_val != nullptr)
		{
			ret.x = App->scripting->UnboxFloat(x_val);
			ret.y = App->scripting->UnboxFloat(y_val);
		}
	}

	return ret;
}

MonoObject * ScriptingCluster::BoxFloat2(const float2 & val)
{
	MonoObject* ret = nullptr;



	return ret;
}

MonoObject* ScriptingCluster::BoxCollision(PhysicsBody * pb)
{
	MonoObject* ret = nullptr;

	if (pb != nullptr)
	{
		ComponentTransform* component = pb->GetComponentTransform();

		if (component != nullptr)
		{
			GameObject* collided_go = component->GetOwner();

			ScriptingClassInstance* collided_go_instance = collided_go->GetScriptingBridge()->GetGoScriptingInstance();

			ScriptingClassInstance instance = collision_class.CreateWeakInstance();

			if (App->scripting->SetFieldValue(instance.GetMonoObject(), collision_class.GetMonoClass(),
				"_go_collided", collided_go_instance->GetMonoObject()))
			{
				ret = instance.GetMonoObject();
			}
		}
	}

	return ret;
}
