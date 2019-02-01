#include "ModuleState.h"
#include "Event.h"
#include "ModuleEvent.h"

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

bool ModuleState::CleanUp()
{
	bool ret = true;

	return ret;
}

void ModuleState::SetEditorUpdateState(EditorUpdateState state)
{
	if (editor_state != state)
	{
		editor_state = state;

		switch (editor_state)
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
			break;
		}
		case EDITOR_UPDATE_STATE_PAUSED:
		{
			EventEditorGoesToPaused* egp = new EventEditorGoesToPaused();
			App->event->SendEvent(egp);
			break;
		}
		}
	}
}

EditorUpdateState ModuleState::GetEditorUpdateState() const
{
	return editor_state;
}
