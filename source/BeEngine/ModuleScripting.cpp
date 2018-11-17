#include "ModuleScripting.h"
#include "App.h"
#include "ModuleProject.h"
#include "ModuleFileSystem.h"
#include "Event.h"
#include "ModuleEvent.h"
#include "ScriptingObjectCompiler.h"
#include "ScriptingObjectSolutionManager.h"
#include "ScriptingObjectFileWatcher.h"
#include <mono/utils/mono-logger.h>

// Remove this 
#include "ModuleResource.h"

#pragma comment (lib, "../Resources/mono/lib/mono-2.0-sgen.lib")

ModuleScripting::ModuleScripting()
{
}

ModuleScripting::~ModuleScripting()
{
}

void MonoLogToLog(const char * log_domain, const char * log_level, const char * message, mono_bool fatal, void * user_data)
{
	if (fatal || log_level == "error")
	{
		CONSOLE_ERROR("%s%s", log_domain != nullptr ? ((std::string)log_domain + ": ").c_str() : "", message);
	}
	else if (log_level == "warning")
	{
		CONSOLE_WARNING("%s%s", log_domain != nullptr ? ((std::string)log_domain + ": ").c_str() : "", message);
	}
	else if (log_level == "critical")
	{
		CONSOLE_ERROR("%s%s", log_domain != nullptr ? ((std::string)log_domain + ": ").c_str() : "", message);
	}
	else
	{

	}
}

void MonoInternalWarning(const char * string, mono_bool is_stdout)
{
	CONSOLE_WARNING("%s", string);
}

void MonoInternalError(const char * string, mono_bool is_stdout)
{
	CONSOLE_ERROR("%s", string);
}

bool ModuleScripting::Awake()
{
	bool ret = true;

	INTERNAL_LOG("Init Mono Envirnoment");

	App->event->Suscribe(std::bind(&ModuleScripting::OnEvent, this, std::placeholders::_1), EventType::PROJECT_SELECTED);

	mono_trace_set_log_handler(MonoLogToLog, this);
	mono_trace_set_print_handler(MonoInternalWarning);
	mono_trace_set_printerr_handler(MonoInternalError);

	mono_base_path = App->file_system->GetWorkingDirectory() + "mono\\";
	assembly_base_path = App->file_system->GetWorkingDirectory() + "mono_assembly\\";

	std::string lib_dir = mono_base_path + "lib\\";
	std::string etc_dir = mono_base_path + "etc\\";
	mono_set_dirs(lib_dir.c_str(), etc_dir.c_str());

	base_domain = mono_jit_init(App->GetAppName());

	std::string scripting_assembly_path = assembly_base_path + "BeEngineScripting.dll";
	std::string scripting_internal_assembly_path = assembly_base_path + "BeEngineScriptingInternal.dll";

	scripting_assembly = CreateAssembly(scripting_assembly_path.c_str());
	scripting_internal_assembly = CreateAssembly(scripting_internal_assembly_path.c_str());

	compiler = (ScriptingObjectCompiler*)AddScriptingObject(new ScriptingObjectCompiler());
	solution_manager = (ScriptingObjectSolutionManager*)AddScriptingObject(new ScriptingObjectSolutionManager());
	file_watcher = (ScriptingObjectFileWatcher*)AddScriptingObject(new ScriptingObjectFileWatcher());

	//MonoClass* test_class = GetMonoClass(base_project_assembly, "BeEngine", "BeEngineReference");

	//CreatedMonoObject obj = CreateMonoObject(test_class);

	//MonoMethod* init_method = GetMonoMethod(test_class, "SetPtr", 1);

	//class PeneClasse
	//{
	//public:
	//	int pene = 5;
	//};

	//PeneClasse* pc = new PeneClasse();
	//pc->pene = 10;

	//float* test = new float[1];
	//test[0] = 4;

	//void *args[1];
	//*args = pc;

	//float* tp = (float*)*args;

	//void* ret_obj = *args;
	//
	//tp = (float*)ret_obj;

	//// --

	//MonoObject* exception = nullptr;

	//MonoObject* ret_oj = nullptr;
	//ret_oj = mono_runtime_invoke(init_method, obj.GetMonoObject(), args, &exception);

	//ret_obj = mono_object_unbox(ret_oj);

	//// --

	//PeneClasse* pc2 = (PeneClasse*)ret_obj;

	return ret;
}

bool ModuleScripting::Start()
{
	bool ret = true;

	return ret;
}

bool ModuleScripting::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool ModuleScripting::Update()
{
	bool ret = true;

	return ret;
}

bool ModuleScripting::PostUpdate()
{
	bool ret = true;

	UpdateScriptingObjects();

	return ret;
}

bool ModuleScripting::CleanUp()
{
	bool ret = true;

	DestroyAllScriptingObjects();

	DestroyAllAssemblys();

	mono_jit_cleanup(base_domain);

	return ret;
}

void ModuleScripting::OnEvent(Event * ev)
{
	switch (ev->GetType())
	{
	case EventType::PROJECT_SELECTED:
	{
		EventProjectSelected* pje = (EventProjectSelected*)ev;

		InitScriptingSolution();

		//wApp->resource->LoadFileToEngine("C:\\Users\\Guillem\\Desktop\\Testing\\ConsoleApp1\\ConsoleApp1\\Test.cs");

		break;
	}
	default:
		break;
	}
}

ScriptingObject* ModuleScripting::AddScriptingObject(ScriptingObject * obj)
{
	ScriptingObject* ret = nullptr;

	if (obj != nullptr)
	{
		if (!obj->loaded)
		{
			obj->RegisterInternalCalls();
			obj->Start();

			scripting_objects.push_back(obj);

			obj->loaded = true;
		}

		ret = obj;
	}

	return ret;
}

void ModuleScripting::DestroyScriptingObject(ScriptingObject* obj)
{
	if (obj != nullptr)
	{
		for (std::vector<ScriptingObject*>::iterator it = scripting_objects.begin(); it != scripting_objects.end(); ++it)
		{
			if ((*it) == obj)
			{
				(*it)->CleanUp();
				RELEASE(*it);
				scripting_objects.erase(it);
				break;
			}
		}
	}
}

ScriptingAssembly* ModuleScripting::CreateAssembly(const char * assembly_path)
{
	ScriptingAssembly* ret = nullptr;

	ret = new ScriptingAssembly(base_domain, assembly_path);
	ret->LoadAssembly();

	if (ret->GetAssemblyLoaded())
	{
		assemblys.push_back(ret);
	}
	else
	{
		RELEASE(ret);
		ret = nullptr;
	}

	return ret;
}
void ModuleScripting::DestroyAssembly(ScriptingAssembly* assembly)
{
	for (std::vector<ScriptingAssembly*>::iterator it = assemblys.begin(); it != assemblys.end(); ++it)
	{
		if ((*it) == assembly)
		{
			RELEASE(*it);
			assemblys.erase(it);
			break;
		}
	}
}
std::vector<ScriptingAssembly*> ModuleScripting::GetScriptingAssemblys() const
{
	return assemblys;
}
//
//bool ModuleScripting::CompileScript(const char * filepath, std::string & compile_errors)
//{
//	//if (compiler_assembly->GetAssemblyLoaded())
//	//{
//	//	MonoClass* compiler_class = compiler_assembly->GetClass("BeEngineScriptCompiler", "CSharpCompiler");
//
//	//	MonoMethod* compile_method = GetMonoMethod(compiler_class, "CompileScript", 3);
//
//	//	void* args[3];
//	//	const char* script_path = "";
//	//	const char* script_name = "";
//	//	
//	//	//args[0] = &value;
//
//
//
//	//	//string script_path, string script_name, ref List<string> compile_errors
//	//}
//
//	return false;
//}

MonoClass * ModuleScripting::GetMonoClass(MonoObject * obj)
{
	MonoClass* ret = nullptr;

	if (obj != nullptr)
	{
		ret = mono_object_get_class(obj);
	}

	return ret;
}

MonoType* ModuleScripting::GetMonoType(MonoClass * mono_class)
{
	MonoType* ret = nullptr;

	if (mono_class != nullptr)
		ret = mono_class_get_type(mono_class);

	return ret;
}

const char * ModuleScripting::GetMonoTypeName(MonoType * mono_type)
{
	const char* ret = nullptr;

	if (mono_type != nullptr)
		ret = mono_type_get_name(mono_type);

	return ret;
}

MonoString* ModuleScripting::BoxString(const char * val)
{
	MonoString* ret = nullptr;

	ret = mono_string_new(base_domain, val);

	return ret;
}

MonoObject* ModuleScripting::BoxBool(bool val)
{
	MonoObject* ret = nullptr;

	ret = mono_value_box(base_domain, mono_get_boolean_class(), &val);

	return ret;
}

MonoObject* ModuleScripting::BoxInt(int val)
{
	MonoObject* ret = nullptr;

	ret = mono_value_box(base_domain, mono_get_int32_class(), &val);

	return ret;
}

MonoObject* ModuleScripting::BoxUint(uint val)
{
	MonoObject* ret = nullptr;

	ret = mono_value_box(base_domain, mono_get_uint32_class(), &val);

	return ret;
}

MonoObject* ModuleScripting::BoxFloat(float val)
{
	MonoObject* ret = nullptr;

	ret = mono_value_box(base_domain, mono_get_single_class(), &val);

	return ret;
}

MonoObject* ModuleScripting::BoxFloat2(const float2& val)
{
	return nullptr;
}

MonoArray* ModuleScripting::BoxArray(MonoClass * objects_mono_class, const std::vector<MonoObject*>& vec)
{
	MonoArray* ret = nullptr;

	if (objects_mono_class != nullptr)
	{
		ret = mono_array_new(base_domain, objects_mono_class, vec.size());

		if (ret != nullptr)
		{
			int index = 0;
			for (std::vector<MonoObject*>::const_iterator it = vec.begin(); it != vec.end(); ++it)
			{
				mono_array_set(ret, MonoObject*, index, *it);

				++index;
			}
		}
	}

	return ret;
}

std::string ModuleScripting::UnboxString(MonoString* val)
{
	std::string ret = "";

	if (val != nullptr)
	{
		ret = mono_string_to_utf8(val);
	}

	return ret;
}

bool ModuleScripting::UnboxBool(MonoObject * val)
{
	bool ret = false;

	if (val != nullptr)
	{
		ret = *(bool*)mono_object_unbox(val);
	}

	return ret;
}

int ModuleScripting::UnboxInt(MonoObject * val)
{
	int ret = 0;

	if (val != nullptr)
	{
		ret = *(int*)mono_object_unbox(val);
	}

	return ret;
}

uint ModuleScripting::UnboxUint(MonoObject * val)
{
	uint ret = 0;

	if (val != nullptr)
	{
		ret = *(uint*)mono_object_unbox(val);
	}

	return ret;
}

float ModuleScripting::UnboxFloat(MonoObject * val)
{
	float ret = 0.0f;

	if (val != nullptr)
	{
		ret = *(float*)mono_object_unbox(val);
	}

	return ret;
}

float2 ModuleScripting::UnboxFloat2(MonoObject * val)
{
	float2 ret = float2::zero;

	if (val != nullptr)
	{

	}

	return ret;
}

std::vector<MonoObject*> ModuleScripting::UnboxArray(MonoClass * mono_class, MonoArray * val)
{
	std::vector<MonoObject*> ret;

	if (val != nullptr)
	{
		if (mono_class != nullptr && val != nullptr)
		{
			uint count = UnboxArrayCount(val);

			for (int i = 0; i < count; ++i)
			{
				MonoObject* obj = mono_array_get(val, MonoObject*, i);

				ret.push_back(obj);
			}
		}
	}

	return ret;
}

uint ModuleScripting::UnboxArrayCount(MonoArray * val)
{
	uint ret = 0;

	if (val != nullptr)
	{
		ret = mono_array_length(val);
	}

	return ret;
}

void ModuleScripting::InitScriptingSolution()
{
	solution_manager->CreateSolutionManagerInstance();

	solution_manager->RemoveAllAssemblys();
	solution_manager->RemoveAllScripts();

	std::string base_libs_folder = assembly_base_path + "base_libs\\";

	std::vector<std::string> base_libs = App->file_system->GetFilesInPath(base_libs_folder.c_str());

	for (std::vector<std::string>::iterator it = base_libs.begin(); it != base_libs.end(); ++it)
	{
		std::string curr_lib = (*it);

		solution_manager->AddAssembly(curr_lib.c_str());
	}

	for (std::vector<ScriptingAssembly*>::iterator it = assemblys.begin(); it != assemblys.end(); ++it)
	{
		std::string curr_lib = (*it)->GetPath();

		solution_manager->AddAssembly(curr_lib.c_str());
	}
}

void ModuleScripting::UpdateScriptingObjects()
{
	for (std::vector<ScriptingObject*>::iterator it = scripting_objects.begin(); it != scripting_objects.end(); ++it)
	{
		(*it)->Update();
	}
}

void ModuleScripting::DestroyAllAssemblys()
{
	for (std::vector<ScriptingAssembly*>::iterator it = assemblys.begin(); it != assemblys.end(); ++it)
	{		
		RELEASE(*it);		
	}

	assemblys.clear();
}

void ModuleScripting::DestroyAllScriptingObjects()
{
	for (std::vector<ScriptingObject*>::iterator it = scripting_objects.begin(); it != scripting_objects.end(); ++it)
	{
		(*it)->CleanUp();
		RELEASE(*it);
	}

	scripting_objects.clear();
}

//bool ModuleScripting::GetIsSolutionCreated(const char* project_path, const char* solution_path)
//{
//	bool ret = false;
//
//	if (App->file_system->FileExists(project_path) && App->file_system->FileExists(solution_path))
//		ret = true;
//
//	return ret;
//}
//
//bool ModuleScripting::CreateSolution()
//{
//	bool ret = false;
//
//	/*std::string proj_path = App->project->GetCurrProjectBasePath() + "Assembly-CSharp.csproj";
//	std::string solution_path = App->project->GetCurrProjectBasePath() + "SolutionTest.sln";
//
//	if (!GetIsSolutionCreated(proj_path.c_str(), solution_path.c_str()))
//	{
//		std::string proj_template_path = project_template_base_path + "Assembly-CSharp.csproj";
//		std::string solution_template_path = project_template_base_path + "SolutionTest.sln";
//
//		App->file_system->FileCopyPaste(proj_template_path.c_str(), App->project->GetCurrProjectBasePath().c_str(), true);
//		App->file_system->FileCopyPaste(solution_template_path.c_str(), App->project->GetCurrProjectBasePath().c_str(), true);
//
//		ret = true;
//	}
//*/
//	return ret;
//}

//bool ModuleScripting::InitSolution()
//{
//	bool ret = false;
//
//	/*if (compiler_assembly != nullptr && compiler_assembly->GetAssemblyLoaded())
//	{
//		MonoClass* solution_manager_class = GetMonoClass(compiler_assembly, "BeEngineScriptCompiler", "CSharpSolutionManager");
//
//		solution_manager_object = CreateMonoObject(solution_manager_class);
//
//		MonoMethod* init_method = GetMonoMethod(solution_manager_class, "Init", 1);
//
//		std::string proj_path = App->project->GetCurrProjectBasePath() + "Assembly-CSharp.csproj";
//
//		MonoString* path_mono_string = MonoStringFromString(proj_path.c_str());
//
//		void *args[1];
//		args[0] = path_mono_string;
//
//		if (solution_manager_object.GetLoaded())
//		{
//			MonoObject* ret_obj = nullptr;
//			InvokeMonoMethod(solution_manager_object.GetMonoObject(), init_method, args, ret_obj);
//
//			ret = BoolFromMonoBool((MonoBoolean*)ret_obj);
//		}
//	}*/
//
//	return ret;
//}

ScriptingAssembly::ScriptingAssembly(MonoDomain* dom, const char * assembly_path)
{
	domain = dom;
	path = assembly_path;
}

void ScriptingAssembly::LoadAssembly()
{
	if (!loaded)
	{
		if (domain != nullptr)
		{
			assembly = mono_domain_assembly_open(domain, path.c_str());

			if (assembly != nullptr)
			{
				image = mono_assembly_get_image(assembly);

				if(image != nullptr)
					loaded = true;
			}
		}
	}
}

bool ScriptingAssembly::GetAssemblyLoaded() const
{
	return loaded;
}

MonoAssembly * ScriptingAssembly::GetAssembly()
{
	return assembly;
}

MonoImage * ScriptingAssembly::GetImage()
{
	return image;
}

const char * ScriptingAssembly::GetPath()
{
	return path.c_str();
}

ScriptingClass ScriptingAssembly::GetClass(const char* class_namepsace, const char* class_name)
{
	ScriptingClass ret;

	if (loaded)
	{
		MonoClass* cl = nullptr;

		cl = mono_class_from_name(image, class_namepsace, class_name);

		if (cl != nullptr)
		{
			ret = ScriptingClass(cl);
		}
	}

	return ret;
}

ScriptingClass::ScriptingClass()
{
}

ScriptingClass::ScriptingClass(MonoClass * _mono_class)
{
	mono_class = _mono_class;
}

const char * ScriptingClass::GetNamespace() const
{
	std::string ret = "";

	if (mono_class != nullptr)
		ret = mono_class_get_namespace(mono_class);

	return ret.c_str();
}

bool ScriptingClass::InvokeStaticMonoMethod(const char * method_name, void ** args, uint args_count, MonoObject *& return_object)
{
	bool ret = false;

	if (mono_class != nullptr)
	{
		MonoMethod* method = mono_class_get_method_from_name(mono_class, method_name, args_count);

		if (method != nullptr)
		{
			MonoObject* exception = nullptr;
			return_object = mono_runtime_invoke(method, NULL, args, &exception);

			if (exception != nullptr)
			{
				mono_print_unhandled_exception(exception);
			}
			else
			{
				ret = true;
			}
		}
	}

	return ret;
}

ScriptingClassInstance* ScriptingClass::CreateInstance()
{
	ScriptingClassInstance* ret = nullptr;

	if (mono_class != nullptr)
	{
		MonoObject* obj = nullptr;
		uint id = 0;

		obj = mono_object_new(App->scripting->base_domain, mono_class);

		if (obj != nullptr)
		{
			id = mono_gchandle_new(obj, true);

			mono_runtime_object_init(obj);

			ret = new ScriptingClassInstance(*this, obj, id);
		}
	}

	return ret;
}

ScriptingClassInstance::ScriptingClassInstance(ScriptingClass _scripting_class, MonoObject * _mono_object, uint _id)
{
	scripting_class = _scripting_class;
	mono_object = _mono_object;
	id = _id;
}

void ScriptingClassInstance::CleanUp()
{
	mono_gchandle_free(id);
}

ScriptingClass ScriptingClassInstance::GetClass()
{
	return scripting_class;
}

bool ScriptingClassInstance::InvokeMonoMethod(const char * method_name, void ** args, uint args_count, MonoObject *& return_object)
{
	bool ret = false;

	if (scripting_class.mono_class != nullptr && mono_object != nullptr)
	{
		MonoMethod* method = mono_class_get_method_from_name(scripting_class.mono_class, method_name, args_count);

		if (method != nullptr)
		{
			MonoObject* exception = nullptr;

			return_object = mono_runtime_invoke(method, mono_object, args, &exception);

			if (exception != nullptr)
			{
				mono_print_unhandled_exception(exception);
			}
			else
			{
				ret = true;
			}
		}
	}

	return ret;
}

bool ScriptingClassInstance::InvokeMonoMethodUnmanaged(const char * method_name, void ** args, uint args_count, void *& return_object)
{
	bool ret = false;

	if (scripting_class.mono_class != nullptr && mono_object != nullptr)
	{
		MonoMethod* method = mono_class_get_method_from_name(scripting_class.mono_class, method_name, args_count);

		if (method != nullptr)
		{
			MonoObject* exception = nullptr;
			return_object = mono_runtime_invoke(method, mono_object, args, &exception);

			if (exception != nullptr)
			{
				mono_print_unhandled_exception(exception);
			}
			else
			{
				ret = true;
			}
		}
	}

	return ret;
}
