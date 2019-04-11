#include "ScriptingBridgeResourceTexture.h"
#include "App.h"
#include "ModuleScripting.h"
#include "ScriptingCluster.h"
#include "ResourceTexture.h"
#include "ScriptingBridgeBeObject.h"

ScriptingBridgeResourceTexture::ScriptingBridgeResourceTexture(ResourceTexture * resource)
	: ScriptingBridgeObject(App->scripting->scripting_cluster->resource_texture_class)

{
	resource_ref = resource;
}

ScriptingBridgeResourceTexture::~ScriptingBridgeResourceTexture()
{
}

void ScriptingBridgeResourceTexture::Start()
{
}

void ScriptingBridgeResourceTexture::OnRebuildInstances()
{
	ScriptingBridgeBeObject::SetBeObjectRefPointer(class_instance->GetMonoObject(), resource_ref);
}

void ScriptingBridgeResourceTexture::CleanUp()
{
}
