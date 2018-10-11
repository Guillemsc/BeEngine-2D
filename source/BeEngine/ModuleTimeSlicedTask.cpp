#include "ModuleTimeSlicedTask.h"

ModuleTimeSlicedTask::ModuleTimeSlicedTask()
{
}

ModuleTimeSlicedTask::~ModuleTimeSlicedTask()
{
}

bool ModuleTimeSlicedTask::Awake()
{
	bool ret = true;

	return ret;
}

bool ModuleTimeSlicedTask::Start()
{
	bool ret = true;

	return ret;
}

bool ModuleTimeSlicedTask::PreUpdate()
{
	bool ret = true;

	UpdateTimeSlicedTasks();

	return ret;
}

bool ModuleTimeSlicedTask::Update()
{
	bool ret = true;

	return ret;
}

bool ModuleTimeSlicedTask::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool ModuleTimeSlicedTask::CleanUp()
{
	bool ret = true;

	return ret;
}

void ModuleTimeSlicedTask::StartTimeSlicedTask(TimeSlicedTask * task)
{
	if (task != nullptr)
	{
		bool exists = false;
		for (std::vector<TimeSlicedTask*>::iterator it = running_tasks.begin(); it != running_tasks.end(); ++it)
		{
			if ((*it) == task)
			{
				exists = true;
				break;
			}
		}

		if (!exists)
		{
			running_tasks.push_back(task);
		}
	}
}

void ModuleTimeSlicedTask::FinishTimeSlicedTask(TimeSlicedTask * task)
{
	for (std::vector<TimeSlicedTask*>::iterator it = running_tasks.begin(); it != running_tasks.end(); ++it)
	{
		if ((*it) == task)
		{
			task->finished = true;
			break;
		}
	}
}

void ModuleTimeSlicedTask::UpdateTimeSlicedTasks()
{
	for (std::vector<TimeSlicedTask*>::iterator it = running_tasks.begin(); it != running_tasks.end();)
	{
		TimeSlicedTask* curr = (*it);

		if (!curr->finished)
		{
			for (int i = 0; i < (*it)->iterations_per_frame; ++i)
			{
				if (curr->first_update)
				{
					if (curr->on_start)
						curr->on_start(*it);

					curr->first_update = false;
				}

				curr->Update();

				if (curr->on_update)
					curr->on_update(*it);
			}

			++it;
		}
		else
		{
			curr->Finish();

			if (curr->on_finish)
				curr->on_finish(curr);

			//Event ev(EventType::ET_ASYNC_TASK_FINISHED);
			//ev.async_task_finished.task = curr;
			//App->event_system->Send(ev);

			RELEASE(*it);

			it = running_tasks.erase(it);
		}
	}
}

TimeSlicedTask::TimeSlicedTask(TimeSlicedTaskType _mode, uint _iterations_per_frame, std::string _task_name)
{
	mode = _mode;
	task_name = _task_name;
	iterations_per_frame = _iterations_per_frame;

	if (iterations_per_frame <= 0)
		iterations_per_frame = 1;
}

void TimeSlicedTask::SetPercentageProgress(float set)
{
	progress = set;

	if (progress < 0)
		progress = 0.0f;

	if (progress > 100)
		progress = 100.0f;
}

float TimeSlicedTask::GetPercentageProgress() const
{
	return progress;
}

void TimeSlicedTask::SetDescription(const std::string & phase)
{
	description = phase;
}

std::string TimeSlicedTask::GetDescription() const
{
	return description;
}

void TimeSlicedTask::FinishTask()
{
	finished = true;
}

TimeSlicedTaskType TimeSlicedTask::GetMode() const
{
	return mode;
}

void TimeSlicedTask::OnStart(const std::function<void(TimeSlicedTask*)>& fun)
{
	on_start = fun;
}

void TimeSlicedTask::OnUpdate(const std::function<void(TimeSlicedTask*)>& fun)
{
	on_update = fun;
}

void TimeSlicedTask::OnFinish(const std::function<void(TimeSlicedTask*)>& fun)
{
	on_finish = fun;
}
