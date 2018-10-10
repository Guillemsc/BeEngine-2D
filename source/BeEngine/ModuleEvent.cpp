#include "ModuleEvent.h"

Event::Event()
{
}

EventDelegate::EventDelegate()
{
}

void EventDelegate::AddListener(std::function<void(const Event& ev)> function)
{
	listeners.push_back(function);
}

void EventDelegate::RemoveListener(std::function<void(const Event&)> function)
{
	for (std::vector<std::function<void(const Event&)>>::iterator it = listeners.begin(); it != listeners.end(); ++it)
	{
		if ((*it).target_type() == function.target_type())
		{
			listeners.erase(it);
			break;
		}
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

	ed.RemoveListener(std::bind(&ModuleEvent::OnEvent, this, std::placeholders::_1));

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

	return ret;
}
