#include "ModuleAction.h"

ModuleAction::ModuleAction()
{
}

ModuleAction::~ModuleAction()
{
}

bool ModuleAction::Awake()
{
	bool ret = true;

	return ret;
}

bool ModuleAction::Start()
{
	bool ret = true;

	return ret;
}

bool ModuleAction::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool ModuleAction::Update()
{
	bool ret = true;

	return ret;
}

bool ModuleAction::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool ModuleAction::CleanUp()
{
	bool ret = true;

	return ret;
}

void ModuleAction::DoAction(Action* action)
{
	if (action != nullptr)
	{
		bool exists = false;
		for (std::vector<Action*>::iterator it = actions_event_list.begin(); it != actions_event_list.end(); ++it)
		{
			if ((*it) == action)
			{
				exists = true;
				break;
			}
		}

		if (!exists)
		{
			int curr_size = actions_event_list.size();

			std::vector<Action*>::iterator it = actions_event_list.begin();

			it += action_position;

			for (it; it != actions_event_list.end();)
			{
				RELEASE(*it);
				it = actions_event_list.erase(it);
			}

			action->DoAction();

			actions_event_list.push_back(action);

			++action_position;
		}
	}
}

void ModuleAction::Undo()
{
	if (CanUndo())
	{	
		actions_event_list[action_position - 1]->UndoAction();

		--action_position;

		if (action_position < 0)
			action_position = 0;
	}
}

void ModuleAction::Redo()
{
	if (CanRedo())
	{		
		++action_position;

		actions_event_list[action_position - 1]->DoAction();
	}
}

bool ModuleAction::CanUndo() const
{
	bool ret = false;

	if (action_position > 0)
	{
		if (action_position <= actions_event_list.size())
		{			
			ret = true;
		}
	}

	return ret;
}

bool ModuleAction::CanRedo() const
{
	bool ret = false;

	if (action_position >= 0)
	{
		if (actions_event_list.size() > action_position)
		{
			ret = true;
		}
	}

	return ret;
}

Action::Action()
{
}
