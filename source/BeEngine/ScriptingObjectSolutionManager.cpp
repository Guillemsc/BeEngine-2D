#include "ScriptingObjectSolutionManager.h"
#include "App.h"
#include "ModuleProject.h"
#include "ModuleFileSystem.h"

ScriptingObjectSolutionManager::ScriptingObjectSolutionManager()
{
}

ScriptingObjectSolutionManager::~ScriptingObjectSolutionManager()
{
}

void ScriptingObjectSolutionManager::Start()
{
	visual_studio_project_template = "templates\\visual_studio_project_template\\";

	if (App->scripting->scripting_internal_assembly != nullptr && App->scripting->scripting_internal_assembly->GetAssemblyLoaded())
	{
		ScriptingClass solution_manager_class = App->scripting->scripting_internal_assembly->GetClass("BeEngine.Internal", "SolutionManager");

		solution_manager_instance = solution_manager_class.CreateInstance();
	}
}

void ScriptingObjectSolutionManager::CleanUp()
{
	if (solution_manager_instance != nullptr)
	{
		solution_manager_instance->CleanUp();
		RELEASE(solution_manager_instance);
	}
}

bool ScriptingObjectSolutionManager::GetSolutionCreated()
{
	bool ret = false;

	if (App->file_system->FileExists(proj_path.c_str()))
	{
		if (App->file_system->FileExists(sol_path.c_str()))
		{
			ret = true;
		}
	}

	return ret;
}

bool ScriptingObjectSolutionManager::CreateSolution()
{
	bool ret = false;

	std::string template_proj_path = visual_studio_project_template + "Assembly-CSharp.csproj";
	std::string template_solution_path = visual_studio_project_template + "SolutionTest.sln";

	bool proj_copied = App->file_system->FileCopyPaste(template_proj_path.c_str(), App->project->GetCurrProjectBasePath().c_str(), true, proj_path);
	bool sol_copied = App->file_system->FileCopyPaste(template_solution_path.c_str(), App->project->GetCurrProjectBasePath().c_str(), true, sol_path);

	if (proj_copied && sol_copied)
		ret = true;

	return ret;
}

bool ScriptingObjectSolutionManager::CreateSolutionManagerInstance()
{
	bool ret = false;

	proj_path = App->project->GetCurrProjectBasePath() + "Assembly-CSharp.csproj";
	sol_path = App->project->GetCurrProjectBasePath() + "SolutionTest.sln";

	bool created = GetSolutionCreated();

	if (!created)
	{
		created = CreateSolution();
	}

	if (solution_manager_instance != nullptr && created)
	{
		MonoString* path_mono_string = App->scripting->BoxString(proj_path.c_str());

		void *args[1];
		args[0] = path_mono_string;

		MonoObject* ret_obj = nullptr;
		if(solution_manager_instance->InvokeMonoMethod("Init", args, 1, ret_obj))
		{
			ready_to_use = App->scripting->UnboxBool(ret_obj);

			if (ready_to_use)
				ret = true;
		}
	}

	return ret;
}

bool ScriptingObjectSolutionManager::AddAssembly(const char * dll_filepath)
{
	bool ret = false;

	if (solution_manager_instance != nullptr && ready_to_use)
	{
		if (solution_manager_instance != nullptr && ready_to_use)
		{
			DecomposedFilePath df = App->file_system->DecomposeFilePath(dll_filepath);

			MonoObject* path_boxed = (MonoObject*)App->scripting->BoxString(dll_filepath);
			MonoObject* name_boxed = (MonoObject*)App->scripting->BoxString(df.file_name.c_str());

			void *args[2];
			args[0] = path_boxed;
			args[1] = name_boxed;

			MonoObject* ret_obj = nullptr;
			if (solution_manager_instance->InvokeMonoMethod("AddAssembly", args, 2, ret_obj))
			{
				ret = App->scripting->UnboxBool(ret_obj);
			}
		}
	}

	return ret;
}

bool ScriptingObjectSolutionManager::AddScript(const char * script_filepath)
{
	bool ret = false;

	if (solution_manager_instance != nullptr && ready_to_use)
	{
		MonoObject* path_boxed = (MonoObject*)App->scripting->BoxString(script_filepath);

		void *args[1];
		args[0] = path_boxed;

		MonoObject* ret_obj = nullptr;
		if (solution_manager_instance->InvokeMonoMethod("AddScript", args, 1, ret_obj))
		{
			ret = App->scripting->UnboxBool(ret_obj);
		}
	}

	return ret;
}

void ScriptingObjectSolutionManager::RemoveScript(const char * script_filepath)
{
	if (solution_manager_instance != nullptr && ready_to_use)
	{
		MonoObject* path_boxes = (MonoObject*)App->scripting->BoxString(script_filepath);

		void *args[1];
		args[0] = path_boxes;

		MonoObject* ret_obj = nullptr;
		if (solution_manager_instance->InvokeMonoMethod("RemoveScript", args, 1, ret_obj))
		{
			App->scripting->UnboxBool(ret_obj);
		}
	}
}
