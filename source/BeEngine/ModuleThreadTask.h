#ifndef __MODULE_THREAD_H__
#define __MODULE_THREAD_H__

#include "Module.h"
#include <thread>
#include <functional>
#include <future>

class ThreadTask
{
	friend class ModuleThreadTask;

public:
	ThreadTask();

	virtual void Start() {};
	virtual void Update() {};
	virtual void Finish() {};

	void FinishTask();

private:
	bool finished = false;
};

class Thread
{
	friend class ModuleThreadTask;

private:
	void operator delete(void *) {}

public:
	Thread(const std::function<bool(ThreadTask*)>& function);
	~Thread();

	void Start();
	bool GetStarted() const;
	bool GetFinished();

private:
	bool started = false;
	bool finished = false;

	std::function<bool(ThreadTask*)> function;
	std::future<bool> future;

	ThreadTask* task = nullptr;
};

class ModuleThreadTask : public Module
{
public:
	ModuleThreadTask();
	~ModuleThreadTask();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

	void StartThread(ThreadTask* task);

private:
	void CheckThreadsStatus();

	bool ThreadFunction(ThreadTask* task);

private:
	std::vector<Thread*> threads;
};

#endif // !__MODULE_THREAD_H__
