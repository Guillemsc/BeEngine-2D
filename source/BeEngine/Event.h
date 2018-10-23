#ifndef __EVENT_H__
#define __EVENT_H__

#include "ModuleTimeSlicedTask.h"
#include "GeometryMath.h"
#include "ModuleThreadTask.h"

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

	THREAD_TASK_FINISHED,

	SCENE_WINDOW_RESIZE,

	PROJECT_SELECTED,
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

	TimeSlicedTask* GetTask() const
	{
		return task;
	};

private:
	TimeSlicedTask * task = nullptr;
};

class EventThreadTaskFinished : public Event
{
public:
	EventThreadTaskFinished(ThreadTask* thread_task) : Event(EventType::THREAD_TASK_FINISHED)
	{
		task = thread_task;
	}

	ThreadTask* GetTask() const
	{
		return task;
	};

private:
	ThreadTask* task = nullptr;
};

class EventSceneWindowResize : public Event
{
public:
	EventSceneWindowResize(float2 last_s, float2 new_s) : Event(EventType::SCENE_WINDOW_RESIZE)
	{
		last_size = last_s;
		new_size = new_s;
	}

	float2 GetLastSize() const
	{
		return last_size;
	};

	float2 GetNewSize() const
	{
		return new_size;
	};

private:
	float2 last_size = float2::zero;
	float2 new_size = float2::zero;
};

class EventProjectSelected : public Event
{
public:
	EventProjectSelected() : Event(EventType::PROJECT_SELECTED)
	{

	}
};

#endif // !__EVENT_H__