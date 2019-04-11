#include "ScriptingBridgeResourceScene.h"
#include "App.h"
#include "ModuleScripting.h"
#include "ScriptingCluster.h"
#include "ResourceScene.h"
#include "ScriptingBridgeBeObject.h"

ScriptingBridgeResourceScene::ScriptingBridgeResourceScene(ResourceScene * resource)
	: ScriptingBridgeObject(App->scripting->scripting_cluster->resource_scene_class)
{
	resource_ref = resource;
}

ScriptingBridgeResourceScene::~ScriptingBridgeResourceScene()
{
}

void ScriptingBridgeResourceScene::Start()
{
}

void ScriptingBridgeResourceScene::OnRebuildInstances()
{
	ScriptingBridgeBeObject::SetBeObjectRefPointer(class_instance->GetMonoObject(), resource_ref);
}

void ScriptingBridgeResourceScene::CleanUp()
{
}
