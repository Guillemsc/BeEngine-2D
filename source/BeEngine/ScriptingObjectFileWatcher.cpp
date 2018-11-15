#include "ScriptingObjectFileWatcher.h"
#include "App.h"
#include "ModuleFileSystem.h"
#include "ModuleEvent.h"

ScriptingObjectFileWatcher::ScriptingObjectFileWatcher()
{
}

ScriptingObjectFileWatcher::~ScriptingObjectFileWatcher()
{
}

void ScriptingObjectFileWatcher::RegisterInternalCalls()
{
	mono_add_internal_call("BeEngine.Internal.FileWatcher::FileFolderChangedCallback", (const void*)FileFolderChangedCallback);
}

void ScriptingObjectFileWatcher::Start()
{
	if (App->scripting->scripting_internal_assembly != nullptr && App->scripting->scripting_internal_assembly->GetAssemblyLoaded())
	{
		ScriptingClass compiler_class = App->scripting->scripting_internal_assembly->GetClass("BeEngine.Internal", "FileWatcher");

		script_file_watcher_instance = compiler_class.CreateInstance();

		if (script_file_watcher_instance != nullptr)
		{
			ready_to_use = true;
		}
	}
}

void ScriptingObjectFileWatcher::CleanUp()
{
	if (script_file_watcher_instance != nullptr)
	{
		script_file_watcher_instance->CleanUp();
		RELEASE(script_file_watcher_instance);
	}
}

bool ScriptingObjectFileWatcher::WatchFileFolder(const char * path)
{
	bool ret = false;

	if (script_file_watcher_instance != nullptr)
	{
		if (App->file_system->FolderExists(path) || App->file_system->FileExists(path))
		{
			MonoObject* boxed_path = (MonoObject*)App->scripting->BoxString(path);

			void *args[1];
			args[0] = boxed_path;

			MonoObject* ret_obj = nullptr;
			script_file_watcher_instance->InvokeMonoMethod("FileFolderWatch", args, 1, ret_obj);

			ret = App->scripting->UnboxBool(ret_obj);

			int i = 0;
		}
	}

	return ret;
}

static void FileFolderChangedCallback(MonoObject* instance, MonoString* str)
{
	if (instance != nullptr && str != nullptr)
	{
		std::string path = App->scripting->UnboxString(str);

		EventWatchFileFolderChanged* new_ev = new EventWatchFileFolderChanged(path);
		App->event->SendEvent(new_ev);
	}
}
