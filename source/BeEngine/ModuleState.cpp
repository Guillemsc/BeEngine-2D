#include "ModuleState.h"
#include "Event.h"
#include "ModuleEvent.h"
#include "ModuleScripting.h"
#include "ModuleAssets.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

ModuleState::ModuleState() : Module()
{
}

ModuleState::~ModuleState()
{
}

bool ModuleState::Awake()
{
	bool ret = true;

	return ret;
}

bool ModuleState::PreUpdate()
{
	bool ret = true;

	ActuallySetEditorState();

	return ret;
}

bool ModuleState::CleanUp()
{
	bool ret = true;

	return ret;
}

void ModuleState::SetEditorUpdateState(EditorUpdateState state)
{
	if (editor_state != state)
	{
		editor_state_to_set = state;

		needs_to_change_state = true;
	}
}

EditorUpdateState ModuleState::GetEditorUpdateState() const
{
	return editor_state;
}

void ModuleState::SetEngineState(EngineState state)
{
	engine_state = state;
}

EngineState ModuleState::GetEngineState() const
{
	return engine_state;
}

float ModuleState::GetTimeSinceStartPlaying() const
{
	return time_since_start_play.ReadSec();
}

void ModuleState::ActuallySetEditorState()
{
	bool can_update = true;

	if (App->scripting->GetNeedsToCompileScripts() || !App->scripting->GetScriptsCompile())
		can_update = false;

	if (can_update)
	{
		if (needs_to_change_state)
		{
			needs_to_change_state = false;

			editor_state = editor_state_to_set;

			switch (editor_state_to_set)
			{
			case EDITOR_UPDATE_STATE_IDLE:
			{
				EventEditorGoesToIdle* egi = new EventEditorGoesToIdle();
				App->event->SendEvent(egi);
				break;
			}
			case EDITOR_UPDATE_STATE_PLAY:
			{
				EventEditorGoesToPlay* egp = new EventEditorGoesToPlay();
				App->event->SendEvent(egp);

				time_since_start_play.Start();

				break;
			}
			}
		}
	}
}
