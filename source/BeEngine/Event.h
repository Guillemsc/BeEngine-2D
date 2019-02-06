#ifndef __EVENT_H__
#define __EVENT_H__

#include "ModuleTimeSlicedTask.h"
#include "GeometryMath.h"
#include "ModuleThreadTask.h"
#include "ModuleFileSystem.h"
#include "App.h"

enum EventType;
class Resource;
class GameObject;

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
	GAME_WINDOW_RESIZE,

	PROJECT_SELECTED,

	EDITOR_GOES_TO_PLAY,
	EDITOR_GOES_TO_IDLE,
	EDITOR_GOES_TO_PAUSED,

	WATCH_FILE_FOLDER,

	RESOURCE_DESTROYED,

	SCRIPTS_COMPILED,
	RESOURCE_SCRIPTS_FIELDS_CHANGED,

	GAME_OBJECT_CREATED,
	GAME_OBJECT_DESTROYED,
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

class EventGameWindowResize : public Event
{
public:
	EventGameWindowResize(float2 last_s, float2 new_s) : Event(EventType::GAME_WINDOW_RESIZE)
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

class EventWatchFileFolderChanged : public Event
{
public:
	EventWatchFileFolderChanged(std::string path) : Event(EventType::WATCH_FILE_FOLDER)
	{
		fp = App->file_system->DecomposeFilePath(path.c_str());
	}

	DecomposedFilePath GetPath()
	{
		return fp;
	}

private:
	DecomposedFilePath fp;
};

class EventResourceDestroyed : public Event
{
public:
	EventResourceDestroyed(Resource* res) : Event(EventType::RESOURCE_DESTROYED)
	{
		resource = res;
	}

	Resource* GetResource()
	{
		return resource;
	}

private:
	Resource* resource = nullptr;
};

class EventScriptsCompiled : public Event
{
public:
	EventScriptsCompiled(bool scripts_compile) : Event(EventType::SCRIPTS_COMPILED)
	{
		compiles = scripts_compile;
	}

	bool GetCompiles()
	{
		return compiles;
	}

private:
	bool compiles = false;
};

class EventResourceScriptsFieldsChanged : public Event
{
public:
	EventResourceScriptsFieldsChanged() : Event(EventType::RESOURCE_SCRIPTS_FIELDS_CHANGED)
	{

	}

private:
	bool compiles = false;
};

class EventGameObjectCreated : public Event
{
public:
	EventGameObjectCreated(GameObject* go) : Event(EventType::GAME_OBJECT_CREATED)
	{
		gameobject = go;
	}

	GameObject* GetGameObject()
	{
		return gameobject;
	}

private:
	GameObject* gameobject = nullptr;
};

class EventGameObjectDestroyed : public Event
{
public:
	EventGameObjectDestroyed(GameObject* go) : Event(EventType::GAME_OBJECT_DESTROYED)
	{
		gameobject = go;
	}

	GameObject* GetGameObject()
	{
		return gameobject;
	}

private:
	GameObject* gameobject = nullptr;
};

class EventEditorGoesToIdle : public Event
{
public:
	EventEditorGoesToIdle() : Event(EventType::EDITOR_GOES_TO_IDLE)
	{
	}
};

class EventEditorGoesToPlay : public Event
{
public:
	EventEditorGoesToPlay() : Event(EventType::EDITOR_GOES_TO_PLAY)
	{

	}
};

class EventEditorGoesToPaused : public Event
{
public:
	EventEditorGoesToPaused() : Event(EventType::EDITOR_GOES_TO_PAUSED)
	{
	}
};


#endif // !__EVENT_H__