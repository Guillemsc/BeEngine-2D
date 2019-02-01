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
	bool CleanUp();

	void SetEditorUpdateState(EditorUpdateState state);
	EditorUpdateState GetEditorUpdateState() const;

private:
	EditorUpdateState editor_state = EditorUpdateState::EDITOR_UPDATE_STATE_IDLE;
};

#endif // !__MODULE_STATE_H__