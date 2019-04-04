#include "ScriptingBridgeDebug.h"
#include "Globals.h"
#include "App.h"
#include "ModuleScripting.h"
#include "ModuleState.h"

ScriptingBridgeDebug::ScriptingBridgeDebug() 
	: ScriptingBridgeObject(nullptr)
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
	if (App->state->GetEngineState() != EngineState::ENGINE_STATE_BUILD)
	{
		std::string log_val = App->scripting->UnboxString(mono_string);

		CONSOLE_INFO(log_val.c_str());
	}
}
