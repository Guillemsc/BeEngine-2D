#include "ScriptingObjectCompiler.h"
#include "App.h"

ScriptingObjectCompiler::ScriptingObjectCompiler() : ScriptingObject()
{

}

ScriptingObjectCompiler::~ScriptingObjectCompiler()
{
}

void ScriptingObjectCompiler::Start()
{
	if (App->scripting->scripting_internal_assembly != nullptr && App->scripting->scripting_internal_assembly->GetAssemblyLoaded())
	{
		ScriptingClass compiler_class = App->scripting->scripting_internal_assembly->GetClass("BeEngine.Internal", "ScriptCompiler");
	
		script_compiler_instance = compiler_class.CreateInstance();
	
		if (script_compiler_instance != nullptr)
		{
			std::vector<ScriptingAssembly*> assemblys = App->scripting->GetScriptingAssemblys();

			std::vector<MonoObject*> assembly_objects;
			for (std::vector<ScriptingAssembly*>::iterator it = assemblys.begin(); it != assemblys.end(); ++it)
			{
				const char* path = (*it)->GetPath();

				MonoObject* obj = (MonoObject*)App->scripting->BoxString(path);

				assembly_objects.push_back(obj);
			}

			MonoArray* assemblys_array = App->scripting->BoxArray(mono_get_string_class(), assembly_objects);

			void *args[1];
			args[0] = assemblys_array;
			
			MonoObject* ret_obj = nullptr;
			script_compiler_instance->InvokeMonoMethod("Init", args, 1, ret_obj);

			bool succes = App->scripting->UnboxBool(ret_obj);

			ready_to_use = succes;
		}
	}
}

void ScriptingObjectCompiler::CleanUp()
{
	if (script_compiler_instance != nullptr)
	{
		script_compiler_instance->CleanUp();
		RELEASE(script_compiler_instance);
	}
}

bool ScriptingObjectCompiler::CompileScript(const char * script_path, const char* dll_output_path, std::vector<std::string>& compile_errors)
{	
	if (App->scripting->scripting_internal_assembly != nullptr && App->scripting->scripting_internal_assembly->GetAssemblyLoaded())
	{
		if (script_compiler_instance != nullptr)
		{
			MonoObject* script_path_boxed = (MonoObject*)App->scripting->BoxString(script_path);
			MonoObject* dll_output_boxed = (MonoObject*)App->scripting->BoxString(dll_output_path);

			void* args[2];
			args[0] = script_path_boxed;
			args[1] = dll_output_boxed;

			MonoObject* ret_obj = nullptr;
			if (script_compiler_instance->InvokeMonoMethod("CompileScript", args, 2, ret_obj))
			{
				if (ret_obj != nullptr)
				{
					std::vector<MonoObject*> rest_vector = App->scripting->UnboxArray(mono_get_string_class(), (MonoArray*)ret_obj);

					for (std::vector<MonoObject*>::iterator it = rest_vector.begin(); it != rest_vector.end(); ++it)
					{
						std::string error = App->scripting->UnboxString((MonoString*)(*it));

						compile_errors.push_back(error);
					}
				}
			}
		}
	}

	return false;
}
