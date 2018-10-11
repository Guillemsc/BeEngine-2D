#ifndef __EVENT_H__
#define __EVENT_H__

#include "ModuleTimeSlicedTask.h"

enum EventType;

class Event
{
public:
	Event(EventType t) { type = t; };

	EventType GetType() const
	{
		return type;
	};

private:
	EventType type;
};

enum EventType
{
	TIME_SLICED_TASK_FINISHED,
	TIME_SLICED_TASK_STARTED,
};

class EventTimeSlicedTaskFinished : public Event
{
public:
	EventTimeSlicedTaskFinished(TimeSlicedTask* sliced_task) : Event(EventType::TIME_SLICED_TASK_FINISHED)
	{
		task = sliced_task;
	}

	TimeSlicedTask* GetTask()
	{
		return task;
	};

private:
	TimeSlicedTask* task = nullptr;
};

class EventTimeSlicedTaskStarted : public Event
{
public:
	EventTimeSlicedTaskStarted(TimeSlicedTask* sliced_task) : Event(EventType::TIME_SLICED_TASK_STARTED)
	{
		task = sliced_task;
	}

	TimeSlicedTask* GetTask()
	{
		return task;
	};

private:
	TimeSlicedTask * task = nullptr;
};

#endif // !__EVENT_H__