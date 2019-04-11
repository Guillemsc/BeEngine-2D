#include "ScriptableObject.h"
#include "App.h"
#include "ModuleScripting.h"

ScriptableObject::ScriptableObject(ScriptingBridgeObject * scripting_brige_object)
{
	scripting_bridge = scripting_brige_object;
}

ScriptableObject::~ScriptableObject()
{
}

void ScriptableObject::StartScriptableObject()
{
	App->scripting->AddScriptingBridgeObject(scripting_bridge);
}

void ScriptableObject::CleanUpScriptableObject()
{
	App->scripting->DestroyScriptingBridgeObject(scripting_bridge);
}

ScriptingBridgeObject * ScriptableObject::GetScriptingBridge() const
{
	return scripting_bridge;
}
