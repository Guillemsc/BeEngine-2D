#include "ScriptingBridgeApplication.h"
#include "App.h"
#include "ModuleState.h"

ScriptingBridgeApplication::ScriptingBridgeApplication()
	: ScriptingBridgeObject(nullptr)
{
}

ScriptingBridgeApplication::~ScriptingBridgeApplication()
{
}

void ScriptingBridgeApplication::Start()
{
}

void ScriptingBridgeApplication::RebuildInstances()
{
}

void ScriptingBridgeApplication::PostRebuildInstances()
{
}

void ScriptingBridgeApplication::CleanUp()
{
}

void ScriptingBridgeApplication::ApplicationQuit()
{
	if (App->state->GetEngineState() == EngineState::ENGINE_STATE_BUILD)
		App->Quit();
	else
		App->state->SetEditorUpdateState(EditorUpdateState::EDITOR_UPDATE_STATE_IDLE);
}
