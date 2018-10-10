#ifndef __MODULE_EVENT_H__
#define __MODULE_EVENT_H__

#include <functional>

#include "Module.h"

class Event
{
public:
	Event();
};

class EventDelegate
{
public: 
	EventDelegate();

	void AddListener(std::function<void(const Event&)> function);
	void RemoveListener(std::function<void(const Event&)> function);

private:
	std::vector<std::function<void(const Event&)>> listeners;
};

class ModuleEvent : public Module
{
public:
	ModuleEvent();
	~ModuleEvent();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

private:
	void OnEvent(const Event& ev) {}

};

#endif // !__MODULE_EVENT_H__