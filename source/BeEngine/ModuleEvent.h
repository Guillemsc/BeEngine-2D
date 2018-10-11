#ifndef __MODULE_EVENT_H__
#define __MODULE_EVENT_H__

#include <functional>
#include <map>

#include "Module.h"
#include "Event.h"

class EventDelegate
{
	friend class ModuleEvent;

private:
	void operator delete(void *) {}

public: 
	EventDelegate(EventType e_type);

	void AddListener(const std::function<void(const Event&)>& function);
	void RemoveListener(const std::function<void(const Event&)>& function);

	void CallListeners(const Event& ev);

private:
	std::vector<std::function<void(const Event&)>> listeners;

	EventType event_type;
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

	void Suscribe(const std::function<void(const Event&)>& function, EventType e_type);
	void UnSuscribe(const std::function<void(const Event&)>& function, EventType e_type);

	void SendEvent(const Event& ev);

private:
	void DestroyAllEventDelegates();
	void OnEvent(const Event& ev) {}

private:
	std::map<int, EventDelegate*> event_delegates;

};

#endif // !__MODULE_EVENT_H__