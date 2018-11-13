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
		MonoClass* compiler_class = App->scripting->GetMonoClass(App->scripting->scripting_internal_assembly, "BeEngine.Internal", "ScriptCompiler");
	
		script_compiler_object = App->scripting->CreateMonoObject(compiler_class);
	
		if (script_compiler_object.GetLoaded())
		{
			MonoMethod* init_method = App->scripting->GetMonoMethod(compiler_class, "Init", 1);

			std::vector<ScriptingAssembly*> assemblys = App->scripting->GetScriptingAssemblys();

			std::vector<MonoObject*> assembly_objects;
			for (std::vector<ScriptingAssembly*>::iterator it = assemblys.begin(); it != assemblys.end(); ++it)
			{
				const char* path = (*it)->GetPath();

				MonoObject* obj = (MonoObject*)App->scripting->MonoStringFromString(path);

				assembly_objects.push_back(obj);
			}

			MonoArray* assemblys_array = App->scripting->MonoArrayFromVector(mono_get_string_class(), assembly_objects);

			void *args[1];
			args[0] = assemblys_array;
			
			MonoObject* ret_obj = nullptr;
			App->scripting->InvokeMonoMethod(script_compiler_object.GetMonoObject(), init_method, args, ret_obj);

			bool succes = App->scripting->BoolFromMonoBool((MonoBoolean*)ret_obj);

			ready_to_use = succes;
		}
	}
}

void ScriptingObjectCompiler::CleanUp()
{
}
