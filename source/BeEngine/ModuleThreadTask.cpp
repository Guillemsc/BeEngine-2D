#include "ModuleThreadTask.h"
#include "App.h"
#include "Event.h"
#include "ModuleEvent.h"

ModuleThreadTask::ModuleThreadTask()
{
}

ModuleThreadTask::~ModuleThreadTask()
{
}

bool ModuleThreadTask::Awake()
{
	bool ret = true;

	return ret;
}

bool ModuleThreadTask::Start()
{
	bool ret = true;

	return ret;
}

bool ModuleThreadTask::PreUpdate()
{
	bool ret = true;

	CheckThreadsStatus();

	return ret;
}

bool ModuleThreadTask::Update()
{
	bool ret = true;

	return ret;
}

bool ModuleThreadTask::PostUpdate()
{
	bool ret = true;

	DeleteThreadsFinished();

	return ret;
}

bool ModuleThreadTask::CleanUp()
{
	bool ret = true;

	return ret;
}

void ModuleThreadTask::StartThread(ThreadTask* task)
{
	if (task != nullptr)
	{
		Thread* thread = new Thread(std::bind(&ModuleThreadTask::ThreadFunction, this, std::placeholders::_1));

		thread->task = task;

		thread->Start();

		threads.push_back(thread);
	}
}

void ModuleThreadTask::StopThread(ThreadTask * task)
{
}

void ModuleThreadTask::CheckThreadsStatus()
{
	for (std::vector<Thread*>::iterator it = threads.begin(); it != threads.end(); )
	{
		if ((*it)->GetFinished())
		{
			finished_threads.push_back(*it);

			it = threads.erase(it);
		}
		else
			++it;
	}
}

void ModuleThreadTask::DeleteThreadsFinished()
{
	for (std::vector<Thread*>::iterator it = finished_threads.begin(); it != finished_threads.end(); )
	{
		App->event->SendEvent(new EventThreadTaskFinished((*it)->task));

		RELEASE(*it);

		it = finished_threads.erase(it);
	}
}

bool ModuleThreadTask::ThreadFunction(ThreadTask * task)
{
	if (task != nullptr)
	{
		task->DoTask();

		return true;
	}

	return false;
}

Thread::Thread(const std::function<bool(ThreadTask*)>& thread_function)
{
	function = thread_function;
}

Thread::~Thread()
{
}

void Thread::Start()
{
	if (!finished)
	{
		future = std::async(std::launch::async, function, task);

		started = true;
		finished = false;
	}
}

bool Thread::GetStarted() const
{
	return started;
}

bool Thread::GetFinished()
{
	bool ret = false;

	if (!finished)
	{
		if (started)
		{
			if (future.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
				finished = true;
		}
	}
	else
		ret = true;

	return ret;
}

ThreadTask::ThreadTask()
{
}
