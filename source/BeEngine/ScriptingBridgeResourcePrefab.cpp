#include "ScriptingBridgeResourcePrefab.h"
#include "App.h"
#include "ModuleScripting.h"
#include "ScriptingCluster.h"
#include "ResourcePrefab.h"
#include "ScriptingBridgeBeObject.h"

ScriptingBridgeResourcePrefab::ScriptingBridgeResourcePrefab(ResourcePrefab * resource)
	: ScriptingBridgeObject(App->scripting->scripting_cluster->resource_prefab_class)
{
	resource_ref = resource;
}

ScriptingBridgeResourcePrefab::~ScriptingBridgeResourcePrefab()
{
}

void ScriptingBridgeResourcePrefab::Start()
{
}

void ScriptingBridgeResourcePrefab::OnRebuildInstances()
{
	ScriptingBridgeBeObject::SetBeObjectRefPointer(class_instance->GetMonoObject(), resource_ref);
}

void ScriptingBridgeResourcePrefab::CleanUp()
{
}
