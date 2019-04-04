#include "ScriptingBridgeTime.h"
#include "App.h"

ScriptingBridgeTime::ScriptingBridgeTime()
	: ScriptingBridgeObject(nullptr)
{
}

ScriptingBridgeTime::~ScriptingBridgeTime()
{
}

void ScriptingBridgeTime::Start()
{
}

void ScriptingBridgeTime::RebuildInstances()
{
}

void ScriptingBridgeTime::PostRebuildInstances()
{
}

void ScriptingBridgeTime::CleanUp()
{
}

float ScriptingBridgeTime::GetDeltaTime()
{
	return App->GetDT();
}
