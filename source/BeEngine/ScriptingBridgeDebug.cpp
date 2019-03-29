#include "ScriptingBridgeDebug.h"
#include "Globals.h"
#include "App.h"
#include "ModuleScripting.h"

ScriptingBridgeDebug::ScriptingBridgeDebug()
{
}

ScriptingBridgeDebug::~ScriptingBridgeDebug()
{
}

void ScriptingBridgeDebug::Start()
{
}

void ScriptingBridgeDebug::RebuildInstances()
{
}

void ScriptingBridgeDebug::PostRebuildInstances()
{
}

void ScriptingBridgeDebug::CleanUp()
{
}

void ScriptingBridgeDebug::Log(MonoString * mono_string)
{
	std::string log_val = App->scripting->UnboxString(mono_string);

	CONSOLE_INFO(log_val.c_str());
}
