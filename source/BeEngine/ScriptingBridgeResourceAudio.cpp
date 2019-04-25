#include "ScriptingBridgeResourceAudio.h"
#include "App.h"
#include "ModuleScripting.h"
#include "ScriptingCluster.h"
#include "ResourceFont.h"
#include "ScriptingBridgeBeObject.h"

ScriptingBridgeResourceAudio::ScriptingBridgeResourceAudio(ResourceAudio * resource)
	: ScriptingBridgeObject(App->scripting->scripting_cluster->resource_audio_class)
{
	resource_ref = resource;
}

ScriptingBridgeResourceAudio::~ScriptingBridgeResourceAudio()
{
}

void ScriptingBridgeResourceAudio::Start()
{
}

void ScriptingBridgeResourceAudio::OnRebuildInstances()
{
	ScriptingBridgeBeObject::SetBeObjectRefPointer(class_instance->GetMonoObject(), resource_ref);
}

void ScriptingBridgeResourceAudio::CleanUp()
{
}
