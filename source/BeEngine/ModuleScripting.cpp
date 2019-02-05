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
#include "ModuleAssets.h"
#include "ScriptingItem.h"
#include "ScriptingItemGameObject.h"
#include "ModuleState.h"
#include <mono/utils/mono-logger.h>
#include <mono/metadata/attrdefs.h>
#include <mono/metadata/mono-config.h>
#include <mono/metadata/threads.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/mono-gc.h>
#include <mono/metadata/environment.h>

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
	App->event->Suscribe(std::bind(&ModuleScripting::OnEvent, this, std::placeholders::_1), EventType::WATCH_FILE_FOLDER);

	mono_trace_set_log_handler(MonoLogToLog, this);
	mono_trace_set_print_handler(MonoInternalWarning);
	mono_trace_set_printerr_handler(MonoInternalError);

	mono_base_path = App->file_system->GetWorkingDirectory() + "mono\\";
	assembly_base_path = App->file_system->GetWorkingDirectory() + "mono_assembly\\";

	std::string base_libs_folder = assembly_base_path + "base_libs\\";
	base_libs = App->file_system->GetFilesInPath(base_libs_folder.c_str());

	std::string lib_dir = mono_base_path + "lib\\";
	std::string etc_dir = mono_base_path + "etc\\";
	mono_set_dirs(lib_dir.c_str(), etc_dir.c_str());

	mono_jit_init(App->GetAppName());

	CreateBaseDomainAndAssemblys();

	return ret;
}

bool ModuleScripting::Start()
{
	bool ret = true;

	App->editor->console_window->AddConsolePersonalLogs("scripting");

	scripting_game_object = (ScriptingItemGameObject*)AddScriptingItem(new ScriptingItemGameObject());

	return ret;
}

bool ModuleScripting::PreUpdate()
{
	bool ret = true;

	ManageScriptsToCompile();

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

	DestroyAllScriptingItems();
	DestroyAllScriptingObjects();

	DestroyAllAssemblys();

	mono_jit_cleanup(mono_get_root_domain());

	App->event->UnSuscribe(std::bind(&ModuleScripting::OnEvent, this, std::placeholders::_1), EventType::PROJECT_SELECTED);
	App->event->UnSuscribe(std::bind(&ModuleScripting::OnEvent, this, std::placeholders::_1), EventType::WATCH_FILE_FOLDER);

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

	case EventType::WATCH_FILE_FOLDER:
	{
		EventWatchFileFolderChanged* efc = (EventWatchFileFolderChanged*)ev;

		DecomposedFilePath df = efc->GetPath();

		if (df.file_extension_lower_case.compare("cs") == 0)
		{
			CompileScripts();
		}

		break;
	}
	}
}

bool ModuleScripting::GetUserCodeCompiles() const
{
	return user_code_compiles;
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

ScriptingItem * ModuleScripting::AddScriptingItem(ScriptingItem * it)
{
	ScriptingItem* ret = nullptr;

	if (it != nullptr)
	{
		if (!it->loaded)
		{
			it->RegisterInternalCalls();
			it->Start();

			scripting_items.push_back(it);

			it->loaded = true;
		}

		ret = it;
	}

	return ret;
}

ScriptingAssembly* ModuleScripting::CreateAssembly(const char * assembly_path, bool used_to_compìle)
{
	ScriptingAssembly* ret = nullptr;

	ret = new ScriptingAssembly(assembly_path, used_to_compìle);
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

	ret = mono_string_new(mono_domain_get(), val);

	return ret;
}

MonoObject* ModuleScripting::BoxBool(bool val)
{
	MonoObject* ret = nullptr;

	ret = mono_value_box(mono_domain_get(), mono_get_boolean_class(), &val);

	return ret;
}

MonoObject* ModuleScripting::BoxInt(int val)
{
	MonoObject* ret = nullptr;

	ret = mono_value_box(mono_domain_get(), mono_get_int32_class(), &val);

	return ret;
}

MonoObject* ModuleScripting::BoxUint(uint val)
{
	MonoObject* ret = nullptr;

	ret = mono_value_box(mono_domain_get(), mono_get_uint32_class(), &val);

	return ret;
}

MonoObject* ModuleScripting::BoxFloat(float val)
{
	MonoObject* ret = nullptr;

	ret = mono_value_box(mono_domain_get(), mono_get_single_class(), &val);

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
		ret = mono_array_new(mono_domain_get(), objects_mono_class, vec.size());

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

MonoArray* ModuleScripting::BoxBuffer(const char * buffer, uint buffer_size)
{
	MonoArray * ret = nullptr;

	if (buffer != nullptr && buffer_size > 0)
	{
		ret = mono_array_new(mono_domain_get(), mono_get_char_class(), buffer_size);

		if (ret != nullptr)
		{
			for (int i = 0; i < buffer_size; ++i)
			{
				char ch = buffer[i];
				MonoObject* boxed_char = mono_value_box(mono_domain_get(), mono_get_char_class(), &ch);

				mono_array_set(ret, MonoObject*, i, boxed_char);
			}
		}
	}

	return ret;
}

MonoArray * ModuleScripting::BoxPointer(void * pointer)
{
	MonoArray* ret = nullptr;

	if (pointer != nullptr)
	{
		char pointer_data[4];	
		memcpy(pointer_data, &pointer, 4);

		ret = App->scripting->BoxBuffer(pointer_data, 4);
	}

	return ret;
}

std::string ModuleScripting::UnboxString(MonoString* val)
{
	std::string ret = "";

	if (val != nullptr)
	{
		char* str = mono_string_to_utf8(val);

		ret = str;

		mono_free(str);
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

std::vector<MonoObject*> ModuleScripting::UnboxArray(MonoArray * val)
{
	std::vector<MonoObject*> ret;

	if (val != nullptr)
	{		
		uint count = UnboxArrayCount(val);

		for (int i = 0; i < count; ++i)
		{
			MonoObject* obj = mono_array_get(val, MonoObject*, i);

			ret.push_back(obj);
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

char * ModuleScripting::UnboxBuffer(MonoArray * val, uint& buffer_size)
{
	char* ret = nullptr;

	if (val != nullptr)
	{		
		buffer_size = UnboxArrayCount(val);

		ret = new char[buffer_size];

		for (int i = 0; i < buffer_size; ++i)
		{
			MonoObject* obj = mono_array_get(val, MonoObject*, i);

			char ch = *(char*)mono_object_unbox(obj);

			ret[i] = ch;
		}
	}

	return ret;
}

void * ModuleScripting::UnboxPointer(MonoArray * val)
{
	void* ret = nullptr;

	if (val != nullptr)
	{
		uint buffer_size = 0;
		const char* ret_str = App->scripting->UnboxBuffer((MonoArray*)val, buffer_size);

		memcpy(&ret, ret_str, 4);

		RELEASE_ARRAY(ret_str);
	}

	return ret;
}

bool ModuleScripting::InvokeMonoMethod(MonoObject * mono_object_ins, MonoClass * mono_class, const char * method_name, void ** args, uint args_count, MonoObject *& return_object)
{
	bool ret = false;

	if (mono_class != nullptr && mono_object_ins != nullptr)
	{
		MonoMethod* method = mono_class_get_method_from_name(mono_class, method_name, args_count);

		if (method != nullptr)
		{
			MonoObject* exception = nullptr;

			return_object = mono_runtime_invoke(method, mono_object_ins, args, &exception);

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

void ModuleScripting::CompileScripts()
{
	needs_to_compile_user_scripts = true;

	compile_user_scripts_timer.Start();
}

void ModuleScripting::ForceCompileScripts()
{
	force_compile_scripts = true;
}

bool ModuleScripting::GetScriptsCompile() const
{
	return user_code_compiles;
}

bool ModuleScripting::GetNeedsToCompileScripts() const
{
	return needs_to_compile_user_scripts;
}

void ModuleScripting::LoadDomain()
{
	if (!has_active_domain)
	{
		mono_domain_set(mono_get_root_domain(), false);

		MonoDomain* new_domain = mono_domain_create_appdomain("BeEngineDomain", NULL);

		mono_domain_set(new_domain, false);

		has_active_domain = true;
	}
}

void ModuleScripting::UnloadDomain()
{
	if (has_active_domain)
	{
		MonoDomain* domain_to_unload = mono_domain_get();

		mono_domain_set(mono_get_root_domain(), false);

		mono_domain_finalize(domain_to_unload, -1);
		mono_domain_unload(domain_to_unload);

		mono_gc_collect(mono_gc_max_generation());

		has_active_domain = false;
	}
}

void ModuleScripting::CreateBaseDomainAndAssemblys()
{
	LoadDomain();

	std::string scripting_assembly_path = assembly_base_path + "BeEngineScripting.dll";
	std::string scripting_internal_assembly_path = assembly_base_path + "BeEngineScriptingInternal.dll";

	scripting_assembly = CreateAssembly(scripting_assembly_path.c_str());
	scripting_internal_assembly = CreateAssembly(scripting_internal_assembly_path.c_str());

	if(user_code_compiles)
		user_code_assembly = CreateAssembly(scripting_user_assembly_filepath.c_str(), false);

	compiler = (ScriptingObjectCompiler*)AddScriptingObject(new ScriptingObjectCompiler());
	solution_manager = (ScriptingObjectSolutionManager*)AddScriptingObject(new ScriptingObjectSolutionManager());
	file_watcher = (ScriptingObjectFileWatcher*)AddScriptingObject(new ScriptingObjectFileWatcher());

	RebuildScriptingItemInstances();
}

void ModuleScripting::DestroyBaseDomainAndAssemblys()
{
	DestroyAllScriptingObjects();
	DestroyAllAssemblys();

	scripting_assembly = nullptr;
	scripting_internal_assembly = nullptr;
	user_code_assembly = nullptr;

	compiler = nullptr;
	solution_manager = nullptr;
	file_watcher = nullptr;

	UnloadDomain();
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
}

void ModuleScripting::ManageScriptsToCompile()
{
	bool update = false;

	if (App->state->GetEditorUpdateState() == EditorUpdateState::EDITOR_UPDATE_STATE_IDLE)
	{
		if (needs_to_compile_user_scripts && compile_user_scripts_timer.ReadSec() > 1.5)
		{
			update = true;
			needs_to_compile_user_scripts = false;
		}

		if (force_compile_scripts)
		{
			update = true;
			force_compile_scripts = false;
		}
	}

	if (update)
	{
		scripting_user_assembly_filepath = App->resource->GetLibraryPathFromResourceType(ResourceType::RESOURCE_TYPE_SCRIPT);
		scripting_user_assembly_filepath += "user_scripting.dll";

		std::vector<std::string> compile_errors;
		std::vector<std::string> compile_warnings;
		user_code_compiles = compiler->CompileScripts(scripting_user_assembly_filepath, compile_errors, compile_warnings);

		App->editor->console_window->ClearPesonalLogs("scripting");
		for (std::vector<std::string>::iterator it = compile_errors.begin(); it != compile_errors.end(); ++it)
		{
			App->editor->console_window->AddPersonalLog("scripting", (*it).c_str(), ConsoleLogType::INTERNAL_LOG_ERROR);
		}

		for (std::vector<std::string>::iterator it = compile_warnings.begin(); it != compile_warnings.end(); ++it)
		{
			App->editor->console_window->AddPersonalLog("scripting", (*it).c_str(), ConsoleLogType::INTERNAL_LOG_WARNING);
		}

		if (user_code_compiles)
		{
			std::vector<std::string> scripts = compiler->GetScripts();

			DestroyBaseDomainAndAssemblys();
			CreateBaseDomainAndAssemblys();

			for (std::vector<std::string>::iterator it = scripts.begin(); it != scripts.end(); ++it)
			{
				compiler->AddScript((*it).c_str());
			}

			solution_manager->CreateSolutionManagerInstance();
		}

		App->assets->StartWatchingFolders();

		EventScriptsCompiled* esc = new EventScriptsCompiled(user_code_compiles);
		App->event->SendEvent(esc);

		needs_to_compile_user_scripts = false;
	}
}

void ModuleScripting::UpdateScriptingObjects()
{
	for (std::vector<ScriptingObject*>::iterator it = scripting_objects.begin(); it != scripting_objects.end(); ++it)
	{
		(*it)->Update();
	}
}

void ModuleScripting::RebuildScriptingItemInstances()
{
	for(std::vector<ScriptingItem*>::iterator it = scripting_items.begin(); it != scripting_items.end(); ++it)
	{
		ScriptingItem* curr_item = (*it);

		curr_item->RebuildInstances();
	}
}

void ModuleScripting::DestroyAllAssemblys()
{
	for (std::vector<ScriptingAssembly*>::iterator it = assemblys.begin(); it != assemblys.end(); ++it)
	{	
		(*it)->CleanUp();
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

void ModuleScripting::DestroyAllScriptingItems()
{
	for (std::vector<ScriptingItem*>::iterator it = scripting_items.begin(); it != scripting_items.end(); ++it)
	{
		(*it)->CleanUp();
		RELEASE(*it);
	}

	scripting_items.clear();
}

ScriptingAssembly::ScriptingAssembly(const char * assembly_path, bool used_to_compile)
{
	path = assembly_path;
	this->used_to_compile = used_to_compile;
}

void ScriptingAssembly::CleanUp()
{
	if (image != nullptr)
		mono_image_close(image);
}

void ScriptingAssembly::LoadAssembly()
{
	if (!loaded)
	{	
		char* assembly_data = nullptr;
		uint assembly_data_size = 0;
		if (App->file_system->FileRead(path, assembly_data, assembly_data_size))
		{
			MonoImageOpenStatus status;
			image = mono_image_open_from_data_with_name(assembly_data, assembly_data_size, true, &status, false, path.c_str());

			if (status == MONO_IMAGE_OK || image != nullptr)
			{
				assembly = mono_assembly_load_from_full(image, path.c_str(), &status, false);

				if (assembly != nullptr)
				{
					loaded = true; 
				}
			}

			RELEASE_ARRAY(assembly_data);
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

		cl = mono_class_from_name(mono_assembly_get_image(assembly), class_namepsace, class_name);

		if (cl != nullptr)
		{
			class_returned = ScriptingClass(cl);

			ret = true;
		}
	}

	return ret;
}

bool ScriptingAssembly::GetUsedToCompile() const
{
	return used_to_compile;
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

MonoClass * ScriptingClass::GetMonoClass() const
{
	return mono_class;
}

bool ScriptingClass::GetParentClass(ScriptingClass & returned_parent_class)
{
	bool ret = false;

	if (mono_class != nullptr)
	{
		MonoClass* parent = mono_class_get_parent(mono_class);

		if (parent != nullptr )
		{
			if (parent != mono_get_object_class())
			{
				returned_parent_class = ScriptingClass(parent);

				ret = true;
			}
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

		obj = mono_object_new(mono_domain_get(), mono_class);

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

bool ScriptingClassInstance::InvokeMonoMethodOnParentClass(ScriptingClass parent_mono_class, const char * method_name, void ** args, uint args_count, MonoObject *& return_object)
{
	bool ret = false;

	if (parent_mono_class.mono_class != nullptr && mono_object != nullptr)
	{
		MonoMethod* method = mono_class_get_method_from_name(parent_mono_class.mono_class, method_name, args_count);

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

bool ScriptingClassInstance::InvokeMonoMethodUnmanagedOnParentClass(ScriptingClass parent_mono_class, const char * method_name, void ** args, uint args_count, void *& return_object)
{
	bool ret = false;

	if (parent_mono_class.mono_class != nullptr && mono_object != nullptr)
	{
		MonoMethod* method = mono_class_get_method_from_name(parent_mono_class.mono_class, method_name, args_count);

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

MonoObject * ScriptingClassInstance::GetMonoObject() const
{
	return mono_object;
}
