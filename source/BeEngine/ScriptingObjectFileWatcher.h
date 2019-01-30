#ifndef __SCRIPTING_OBJECT_FILE_WATCHER_H__
#define __SCRIPTING_OBJECT_FILE_WATCHER_H__

#include "Globals.h"
#include "ScriptingObject.h"
#include "ModuleScripting.h"

class ScriptingObjectFileWatcher : public ScriptingObject
{
	friend class ModuleScripting;

private:
	void operator delete(void *) {}

public:
	ScriptingObjectFileWatcher();
	~ScriptingObjectFileWatcher();

	void RegisterInternalCalls();
	void Start();
	void Update();
	void CleanUp();

	bool Watch(const char* path);
	bool StopWatch(const char* path);
	void SetRiseEvents(bool set);

private:
	void GetChangesStack();

private:
	ScriptingClassInstance* script_file_watcher_instance = nullptr;
};

#endif // !__SCRIPTING_OBJECT_FILE_WATCHER_H__