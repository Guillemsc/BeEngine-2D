#include "ScriptingBridgeInput.h"
#include "ModuleInput.h"
#include "App.h"

ScriptingBridgeInput::ScriptingBridgeInput()
	: ScriptingBridgeObject(nullptr)
{
}

ScriptingBridgeInput::~ScriptingBridgeInput()
{
}

void ScriptingBridgeInput::Start()
{
}

void ScriptingBridgeInput::RebuildInstances()
{
}

void ScriptingBridgeInput::PostRebuildInstances()
{
}

void ScriptingBridgeInput::CleanUp()
{
}

bool ScriptingBridgeInput::GetKeyDown(MonoString * mono_string)
{
	std::string key = App->scripting->UnboxString(mono_string);

	return App->input->GetKeyDown(key.c_str());
}

bool ScriptingBridgeInput::GetKeyRepeat(MonoString * mono_string)
{
	std::string key = App->scripting->UnboxString(mono_string);

	return App->input->GetKeyRepeat(key.c_str());
}

bool ScriptingBridgeInput::GetKeyUp(MonoString * mono_string)
{
	std::string key = App->scripting->UnboxString(mono_string);

	return App->input->GetKeyUp(key.c_str());
}
