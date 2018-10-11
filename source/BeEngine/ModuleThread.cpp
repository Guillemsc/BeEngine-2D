#include "ModuleThread.h"

ModuleThread::ModuleThread()
{
}

ModuleThread::~ModuleThread()
{
}

bool ModuleThread::Awake()
{
	bool ret = true;

	return ret;
}

bool ModuleThread::Start()
{
	bool ret = true;

	return ret;
}

bool ModuleThread::PreUpdate()
{
	bool ret = true;

	CheckThreadsStatus();

	return ret;
}

bool ModuleThread::Update()
{
	bool ret = true;

	return ret;
}

bool ModuleThread::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool ModuleThread::CleanUp()
{
	bool ret = true;



	return ret;
}

void ModuleThread::StartThread(const std::function<bool(void)>& function)
{
	Thread* thread = new Thread(function);

	thread->Start();

	threads.push_back(thread);
}

void ModuleThread::CheckThreadsStatus()
{
	for (std::vector<Thread*>::iterator it = threads.begin(); it != threads.end(); )
	{
		if ((*it)->GetFinished())
		{
			RELEASE(*it);
			it = threads.erase(it);
		}
		else
			++it;
	}
}

Thread::Thread(const std::function<bool(void)>& thread_function)
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
		future = std::async(std::launch::async, function);

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
