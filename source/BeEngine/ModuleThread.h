#ifndef __MODULE_THREAD_H__
#define __MODULE_THREAD_H__

#include "Module.h"
#include <thread>
#include <functional>
#include <future>

class Thread
{
public:
	Thread(const std::function<bool(void)>& function);
	~Thread();

	void Start();
	bool GetStarted() const;
	bool GetFinished();

private:
	bool started = false;
	bool finished = false;

	std::function<bool(void)> function;
	std::future<bool> future;
};

class ModuleThread : public Module
{
public:
	ModuleThread();
	~ModuleThread();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

	void StartThread(const std::function<bool(void)>& function);

private:
	void CheckThreadsStatus();

private:
	std::vector<Thread*> threads;
};

#endif // !__MODULE_THREAD_H__
