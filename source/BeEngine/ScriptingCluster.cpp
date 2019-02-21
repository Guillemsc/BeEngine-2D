#include "ScriptingCluster.h"
#include "App.h"
#include "ModuleScripting.h"
#include "ScriptingBridgeGameObject.h"
#include "ScriptingBridgeComponentTransform.h"

void ScriptingCluster::RegisterInternalCalls()
{
	if (rebuild_internal_calls)
	{
		rebuild_internal_calls = false;

		// GameObject -------------------------------
		mono_add_internal_call("BeEngine.GameObject::SetName", (const void*)ScriptingBridgeGameObject::SetName);
		mono_add_internal_call("BeEngine.GameObject::GetName", (const void*)ScriptingBridgeGameObject::GetName);
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

		// Math
		App->scripting->scripting_assembly->GetClass("BeEngine", "float2", float2_class);

		// GameObject
		App->scripting->scripting_assembly->GetClass("BeEngine", "GameObject", game_object_class);

		// Components
		App->scripting->scripting_assembly->GetClass("BeEngine", "Component", component_class);

		App->scripting->scripting_assembly->GetClass("BeEngine", "ComponentScript", component_script_class);
		App->scripting->scripting_assembly->GetClass("BeEngine", "ComponentTransform", component_transform_class);
	}
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
