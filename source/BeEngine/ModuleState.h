#ifndef __MODULE_STATE_H__
#define __MODULE_STATE_H__

#include "Module.h"

enum EditorUpdateState
{
	EDITOR_UPDATE_STATE_IDLE,
	EDITOR_UPDATE_STATE_PLAY,
	EDITOR_UPDATE_STATE_PAUSED,
};

class ModuleState : public Module
{
public:
	ModuleState();
	~ModuleState();

	bool Awake();
	bool PreUpdate();
	bool CleanUp();

	void SetEditorUpdateState(EditorUpdateState state);
	EditorUpdateState GetEditorUpdateState() const;

private:
	void ActuallySetEditorState();

private:
	EditorUpdateState editor_state = EditorUpdateState::EDITOR_UPDATE_STATE_IDLE;

	EditorUpdateState editor_state_to_set = EditorUpdateState::EDITOR_UPDATE_STATE_IDLE;
	bool needs_to_change_state = false;
};

#endif // !__MODULE_STATE_H__