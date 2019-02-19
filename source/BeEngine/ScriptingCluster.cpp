#include "ScriptingCluster.h"
#include "ScriptingBridgeGameObject.h"
#include "App.h"
#include "ModuleScripting.h"

void ScriptingCluster::RegisterInternalCalls()
{
	if (rebuild_internal_calls)
	{
		rebuild_internal_calls = false;

		// GameObject -------------------------------
		mono_add_internal_call("BeEngine.GameObject::SetName", (const void*)ScriptingBridgeGameObject::SetName);
		mono_add_internal_call("BeEngine.GameObject::GetName", (const void*)ScriptingBridgeGameObject::GetName);
		// ------------------------------- GameObject
	}
}

void ScriptingCluster::RebuildClasses()
{
	if (App->scripting->scripting_assembly != nullptr && App->scripting->scripting_assembly->GetAssemblyLoaded())
	{
		// BeEngineObject
		App->scripting->scripting_assembly->GetClass("BeEngine", "BeEngineObject", beengine_object_class);

		// GameObject
		App->scripting->scripting_assembly->GetClass("BeEngine", "GameObject", game_object_class);

		// Components
		App->scripting->scripting_assembly->GetClass("BeEngine", "Component", component_class);

		App->scripting->scripting_assembly->GetClass("BeEngine", "ComponentScript", component_script_class);
	}
}
