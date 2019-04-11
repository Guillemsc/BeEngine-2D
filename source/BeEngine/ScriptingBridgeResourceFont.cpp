#include "ScriptingBridgeResourceFont.h"
#include "App.h"
#include "ModuleScripting.h"
#include "ScriptingCluster.h"
#include "ResourceFont.h"
#include "ScriptingBridgeBeObject.h"

ScriptingBridgeResourceFont::ScriptingBridgeResourceFont(ResourceFont * resource)
	: ScriptingBridgeObject(App->scripting->scripting_cluster->resource_font_class)
{
	resource_ref = resource;
}

ScriptingBridgeResourceFont::~ScriptingBridgeResourceFont()
{
}

void ScriptingBridgeResourceFont::Start()
{
}

void ScriptingBridgeResourceFont::OnRebuildInstances()
{
	ScriptingBridgeBeObject::SetBeObjectRefPointer(class_instance->GetMonoObject(), resource_ref);
}

void ScriptingBridgeResourceFont::CleanUp()
{
}
