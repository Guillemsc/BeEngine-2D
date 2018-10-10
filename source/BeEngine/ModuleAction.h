#ifndef __MODULE_ACTION_H__
#define __MODULE_ACTION_H__

#include "Module.h"

class Action
{
	friend class ModuleAction;

public:
	Action();

	virtual void DoAction() = 0;
	virtual void UndoAction() = 0;

private:
	void operator delete(void *) {}
};

class ModuleAction : public Module
{
public:
	ModuleAction();
	~ModuleAction();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

	void DoAction(Action* action);

	void Undo();
	void Redo();

	bool CanUndo() const;
	bool CanRedo() const;

private:
	std::vector<Action*> actions_event_list;
	uint action_position = 0;
};

#endif // !__MODULE_ACTION_H__