#include "BeObject.h"
#include "App.h"
#include "ModuleScripting.h"

BeObject::BeObject(ScriptingBridgeObject * bridge)
{
	bridge_object = bridge;
}

BeObject::~BeObject()
{
}

void BeObject::InitBeObject()
{
	App->scripting->AddScriptingBridgeObject(bridge_object);
}

void BeObject::CleanUpBeObject()
{
	App->scripting->DestroyScriptingBridgeObject(bridge_object);
}

ScriptingBridgeObject * BeObject::GetScriptingBridge() const
{
	return bridge_object;
}
