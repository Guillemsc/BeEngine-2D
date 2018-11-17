#ifndef __SCRIPTING_OBJECT_SOLUTION_MANAGER_H__
#define __SCRIPTING_OBJECT_SOLUTION_MANAGER_H__

#include "Globals.h"
#include "ScriptingObject.h"
#include "ModuleScripting.h"

class ScriptingObjectSolutionManager : public ScriptingObject
{
	friend class ModuleScripting;

private:
	void operator delete(void *) {}

public:
	ScriptingObjectSolutionManager();
	~ScriptingObjectSolutionManager();

	void Start();
	void CleanUp();

	bool CreateSolutionManagerInstance();
	bool AddAssembly(const char* dll_filepath);
	bool AddScript(const char* script_filepath);
	void RemoveScript(const char* script_filepath);

private:
	bool GetSolutionCreated();
	bool CreateSolution();

private:
	ScriptingClassInstance* solution_manager_instance = nullptr;

	std::string visual_studio_project_template;
	std::string proj_path;
	std::string sol_path;
};

#endif // !__SCRIPTING_OBJECT_SOLUTION_MANAGER_H__