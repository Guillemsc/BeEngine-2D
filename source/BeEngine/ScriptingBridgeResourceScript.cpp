#include "ScriptingBridgeResourceScript.h"
#include "App.h"
#include "ModuleScripting.h"
#include "ScriptingCluster.h"
#include "ResourceScript.h"
#include "ScriptingBridgeBeObject.h"

ScriptingBridgeResourceScript::ScriptingBridgeResourceScript(ResourceScript * resource)
	: ScriptingBridgeObject(App->scripting->scripting_cluster->resource_script_class)
{
	resource_ref = resource;
}

ScriptingBridgeResourceScript::~ScriptingBridgeResourceScript()
{
}

void ScriptingBridgeResourceScript::Start()
{
}

void ScriptingBridgeResourceScript::OnRebuildInstances()
{
	ScriptingBridgeBeObject::SetBeObjectRefPointer(class_instance->GetMonoObject(), resource_ref);
}

void ScriptingBridgeResourceScript::CleanUp()
{
}
