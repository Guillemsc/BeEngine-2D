#include "ModuleEvent.h"

EventDelegate::EventDelegate(EventType e_type)
{
	event_type = e_type;
}

void EventDelegate::AddListener(const std::function<void(Event* ev)>& function)
{
	listeners.push_back(function);
}

void EventDelegate::RemoveListener(const std::function<void(Event*)>& function)
{
	for (std::vector<std::function<void(Event*)>>::iterator it = listeners.begin(); it != listeners.end(); ++it)
	{
		if ((*it).target_type() == function.target_type())
		{
			listeners.erase(it);
			break;
		}
	}
}

void EventDelegate::CallListeners(Event* ev)
{
	for (std::vector<std::function<void(Event*)>>::iterator it = listeners.begin(); it != listeners.end(); ++it)
	{
		if ((*it))
			(*it)(ev);
	}
}

ModuleEvent::ModuleEvent()
{

}

ModuleEvent::~ModuleEvent()
{
}

bool ModuleEvent::Awake()
{
	bool ret = true;

	return ret;
}

bool ModuleEvent::Start()
{
	bool ret = true;

	return ret;
}

bool ModuleEvent::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool ModuleEvent::Update()
{
	bool ret = true;

	return ret;
}

bool ModuleEvent::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool ModuleEvent::CleanUp()
{
	bool ret = true;

	DestroyAllEventDelegates();

	return ret;
}

void ModuleEvent::Suscribe(const std::function<void(Event*)>& function, EventType e_type)
{
	EventDelegate* ed = event_delegates[e_type];

	if (ed == nullptr)
	{
		ed = new EventDelegate(e_type);

		event_delegates[e_type] = ed;
	}

	ed->AddListener(function);
}

void ModuleEvent::UnSuscribe(const std::function<void(Event*)>& function, EventType e_type)
{
	EventDelegate* ed = event_delegates[e_type];

	if (ed != nullptr)
	{
		ed->RemoveListener(function);
	}
}

void ModuleEvent::SendEvent(Event* ev)
{
	if (ev != nullptr)
	{
		EventDelegate* ed = event_delegates[ev->GetType()];

		if (ed != nullptr)
		{
			ed->CallListeners(ev);
		}

		RELEASE(ev);
	}
}

void ModuleEvent::DestroyAllEventDelegates()
{
	for (std::map<int, EventDelegate*>::iterator it = event_delegates.begin(); it != event_delegates.end(); ++it)
	{
		RELEASE((*it).second);
	}

	event_delegates.clear();
}
