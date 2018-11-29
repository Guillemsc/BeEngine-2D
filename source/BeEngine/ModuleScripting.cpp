#include "ModuleScripting.h"
#include "App.h"
#include "ModuleProject.h"
#include "ModuleFileSystem.h"
#include "Event.h"
#include "ModuleEvent.h"
#include "ScriptingObjectCompiler.h"
#include "ScriptingObjectSolutionManager.h"
#include "ScriptingObjectFileWatcher.h"
#include "ModuleEditor.h"
#include "ConsoleWindow.h"
#include "ModuleResource.h"
#include "Event.h"
#include <mono/utils/mono-logger.h>
#include <mono/metadata/attrdefs.h>

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

	std::string base_libs_folder = assembly_base_path + "base_libs\\";
	base_libs = App->file_system->GetFilesInPath(base_libs_folder.c_str());

	compiler = (ScriptingObjectCompiler*)AddScriptingObject(new ScriptingObjectCompiler());
	solution_manager = (ScriptingObjectSolutionManager*)AddScriptingObject(new ScriptingObjectSolutionManager());
	file_watcher = (ScriptingObjectFileWatcher*)AddScriptingObject(new ScriptingObjectFileWatcher());

	return ret;
}

bool ModuleScripting::Start()
{
	bool ret = true;

	App->editor->console_window->AddConsolePersonalLogs("scripting");

	return ret;
}

bool ModuleScripting::PreUpdate()
{
	bool ret = true;

	ActuallyCompileScripts();

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

	App->event->UnSuscribe(std::bind(&ModuleScripting::OnEvent, this, std::placeholders::_1), EventType::PROJECT_SELECTED);

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

std::vector<std::string> ModuleScripting::GetBaseLibs() const
{
	return base_libs;
}

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

void ModuleScripting::CompileScripts()
{
	needs_to_compile_user_scripts = true;
}

bool ModuleScripting::GetScriptsCompile() const
{
	return user_code_compiles;
}

void ModuleScripting::InitScriptingSolution()
{
	solution_manager->CreateSolutionManagerInstance();

	solution_manager->RemoveAllAssemblys();
	solution_manager->RemoveAllScripts();

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

	scripting_user_assembly_filepath = App->resource->GetLibraryPathFromResourceType(ResourceType::RESOURCE_TYPE_SCRIPT);
	scripting_user_assembly_filepath += "user_scripting.dll";

	compiler->SetScriptsAssemblyOutputFilepath(scripting_user_assembly_filepath.c_str());
}

void ModuleScripting::GenerateUserCodeAssembly()
{
	if (user_code_assembly != nullptr)
		DestroyAssembly(user_code_assembly);

	user_code_assembly = nullptr;

	if (App->file_system->FileExists(scripting_user_assembly_filepath))
		user_code_assembly = CreateAssembly(scripting_user_assembly_filepath.c_str());
}

void ModuleScripting::ActuallyCompileScripts()
{
	if (needs_to_compile_user_scripts)
	{
		std::vector<std::string> compile_errors;
		user_code_compiles = compiler->CompileScripts(compile_errors);

		App->editor->console_window->ClearPesonalLogs("scripting");
		for (std::vector<std::string>::iterator it = compile_errors.begin(); it != compile_errors.end(); ++it)
		{
			App->editor->console_window->AddPersonalLog("scripting", (*it).c_str(), ConsoleLogType::INTERNAL_LOG_ERROR);
		}

		if (user_code_compiles)
			GenerateUserCodeAssembly();

		EventScriptsCompiled* esc = new EventScriptsCompiled(user_code_compiles);
		App->event->SendEvent(esc);
	}
	
	needs_to_compile_user_scripts = false;
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

bool ScriptingAssembly::GetClass(const char* class_namepsace, const char* class_name, ScriptingClass& class_returned)
{
	bool ret = false;

	if (loaded)
	{
		MonoClass* cl = nullptr;

		cl = mono_class_from_name(image, class_namepsace, class_name);

		if (cl != nullptr)
		{
			class_returned = ScriptingClass(cl);

			ret = true;
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

std::string ScriptingClass::GetNamespace() const
{
	std::string ret = "";

	if (mono_class != nullptr)
		ret = mono_class_get_namespace(mono_class);

	return ret;
}

std::string ScriptingClass::GetName() const
{
	std::string ret = "";

	if (mono_class != nullptr)
		ret = mono_class_get_name(mono_class);

	return ret;
}

bool ScriptingClass::GetParentClass(ScriptingClass & returned_parent_class)
{
	bool ret = false;

	if (mono_class != nullptr)
	{
		MonoClass* parent = mono_class_get_parent(mono_class);

		if (parent != nullptr)
		{
			returned_parent_class = ScriptingClass(parent);

			ret = true;
		}
	}

	return ret;
}

bool ScriptingClass::GetIsInheritedFrom(const ScriptingClass & class_parent)
{
	bool ret = false;

	if (mono_class != nullptr)
	{
		std::string class_parent_name = class_parent.GetName();

		bool check = true;

		while (check)
		{
			ScriptingClass parent_class_to_check;
			check = GetParentClass(parent_class_to_check);

			if (check)
			{
				std::string parent_class_to_check_name = parent_class_to_check.GetName();

				if (class_parent_name.compare(parent_class_to_check_name) == 0)
				{
					ret = true;
					break;
				}
			}
		}
	}

	return ret;
}

std::map<std::string, MonoType*> ScriptingClass::GetFields()
{
	std::map<std::string, MonoType*> ret;

	if (mono_class != nullptr)
	{
		bool iterate = true;

		void* fields_iter = nullptr;

		while (iterate)
		{
			MonoClassField* curr_field = mono_class_get_fields(mono_class, &fields_iter);

			if (curr_field != nullptr)
			{
				if (!(mono_field_get_flags(curr_field) & MONO_METHOD_ATTR_PUBLIC))
				{
					continue;
				}
				else
				{
					std::string property_name = mono_field_get_name(curr_field);
					MonoType* property_type = mono_field_get_type(curr_field);

					ret[property_name] = property_type;
				}
			}
			else
				iterate = false;
		}
	}

	return ret;
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
