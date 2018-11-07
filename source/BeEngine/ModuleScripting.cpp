#include "ModuleScripting.h"
#include "App.h"

#pragma comment (lib, "../Resources/mono/lib/mono-2.0-sgen.lib")

ModuleScripting::ModuleScripting()
{
}

ModuleScripting::~ModuleScripting()
{
}

bool ModuleScripting::Awake()
{
	bool ret = true;

	INTERNAL_LOG("Init Mono Envirnoment");

	mono_base_path = "mono\\";
	assembly_base_path = "mono_assembly\\";

	std::string lib_dir = mono_base_path + "lib\\";
	std::string etc_dir = mono_base_path + "etc\\";
	mono_set_dirs(lib_dir.c_str(), etc_dir.c_str());

	base_domain = mono_jit_init(App->GetAppName());

	std::string assembly_base_project_path = assembly_base_path + "BeEngineCSharp.dll";
	std::string assembly_compiler_path = assembly_base_path + "BeEngineScriptCompiler.dll";

	base_project_assembly = CreateAssembly(assembly_base_project_path.c_str());
	compiler_assembly = CreateAssembly(assembly_compiler_path.c_str());

	if (compiler_assembly == nullptr)
	{
		INTERNAL_LOG("Mono compiler assembly not loaded");

		ret = false;
	}

	if (!InitCompiler())
	{
		INTERNAL_LOG("Mono compiler not loaded");
	}

	MonoClass* test_class = GetMonoClass(base_project_assembly, "BeEngine", "BeEngineReference");

	CreatedMonoObject obj = CreateMonoObject(test_class);

	MonoMethod* init_method = GetMonoMethod(test_class, "SetPtr", 1);

	class PeneClasse
	{
	public:
		int pene = 5;
	};

	PeneClasse* pc = new PeneClasse();
	pc->pene = 10;

	float* test = new float[1];
	test[0] = 4;

	void *args[1];
	*args = pc;

	float* tp = (float*)*args;

	void* ret_obj = *args;
	
	tp = (float*)ret_obj;

	// --

	MonoObject* exception = nullptr;

	MonoObject* ret_oj = nullptr;
	ret_oj = mono_runtime_invoke(init_method, obj.GetMonoObject(), args, &exception);

	ret_obj = mono_object_unbox(ret_oj);

	// --

	PeneClasse* pc2 = (PeneClasse*)ret_obj;

	INTERNAL_LOG("sdl");

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

	return ret;
}

bool ModuleScripting::CleanUp()
{
	bool ret = true;

	DestroyMonoObject(compiler_object);

	mono_jit_cleanup(base_domain);

	return ret;
}

bool ModuleScripting::InitCompiler()
{
	bool ret = false;

	if (compiler_assembly != nullptr && compiler_assembly->GetAssemblyLoaded())
	{
		MonoClass* compiler_class = GetMonoClass(compiler_assembly, "BeEngineScriptCompiler", "CSharpCompiler");

		compiler_object = CreateMonoObject(compiler_class);

		MonoMethod* init_method = GetMonoMethod(compiler_class, "InitCompiler", 1);

		std::vector<MonoObject*> assemblys_objects;
		for (std::vector<ScriptingAssembly*>::iterator it = assemblys.begin(); it != assemblys.end(); ++it)
		{
			const char* path = (*it)->GetPath();

			MonoObject* obj = (MonoObject*)MonoStringFromString(path);

			assemblys_objects.push_back(obj);
		}

		MonoArray* assemblys_array = MonoArrayFromVector(mono_get_string_class(), assemblys_objects);

		void *args[1];
		args[0] = assemblys_array;

		if (compiler_object.GetLoaded())
		{
			MonoObject* ret_obj = nullptr;
			InvokeMonoMethod(compiler_object.GetMonoObject(), init_method, args, ret_obj);

			ret = BoolFromMonoBool((MonoBoolean*)ret_obj);
		}
	}

	return ret;
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

bool ModuleScripting::CompileScript(const char * filepath, std::string & compile_errors)
{
	if (compiler_assembly->GetAssemblyLoaded())
	{
		MonoClass* compiler_class = compiler_assembly->GetClass("BeEngineScriptCompiler", "CSharpCompiler");

		MonoMethod* compile_method = GetMonoMethod(compiler_class, "CompileScript", 3);

		void* args[3];
		const char* script_path = "";
		const char* script_name = "";
		
		//args[0] = &value;



		//string script_path, string script_name, ref List<string> compile_errors
	}

	return false;
}

MonoClass* ModuleScripting::GetMonoClass(ScriptingAssembly * assembly, const char * class_namepsace, const char * class_name)
{
	MonoClass* ret = nullptr;

	if (assembly != nullptr)
	{
		if (assembly->GetAssemblyLoaded())
		{
			ret = assembly->GetClass(class_namepsace, class_name);
		}
	}

	return ret;
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

const char * ModuleScripting::GetMonoClassNamespace(MonoClass * mono_class)
{
	std::string ret = "";

	if (mono_class != nullptr)
		ret = mono_class_get_namespace(mono_class);

	return ret.c_str();
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

MonoMethod* ModuleScripting::GetMonoMethod(MonoClass * mono_class, const char * method_name, uint args_count)
{
	MonoMethod* ret = nullptr;

	if (mono_class != nullptr)
		ret = mono_class_get_method_from_name(mono_class, method_name, args_count);

	return ret;
}

bool ModuleScripting::InvokeStaticMonoMethod(MonoMethod* mono_method, void ** args, MonoObject *& return_object)
{
	bool ret = false;

	if (mono_method != nullptr)
	{
		MonoObject* exception = nullptr;
		return_object = mono_runtime_invoke(mono_method, NULL, args, &exception);

		if (exception != nullptr)
		{
			mono_print_unhandled_exception(exception);
		}
		else
		{
			ret = true;
		}
	}

	return ret;
}

bool ModuleScripting::InvokeMonoMethod(MonoObject* obj, MonoMethod * mono_method, void ** args, MonoObject *& return_object)
{
	bool ret = false;

	if (obj != nullptr && mono_method != nullptr)
	{
		MonoObject* exception = nullptr;
		return_object = mono_runtime_invoke(mono_method, obj, args, &exception);

		if (exception != nullptr)
		{
			mono_print_unhandled_exception(exception);
		}
		else
		{
			ret = true;
		}
	}

	return ret;
}

bool ModuleScripting::InvokeMonoMethodUnmanaged(MonoObject * obj, MonoMethod * mono_method, void ** args, void *& return_object)
{
	bool ret = false;

	if (obj != nullptr && mono_method != nullptr)
	{
		MonoObject* exception = nullptr;
		return_object = mono_runtime_invoke(mono_method, obj, args, &exception);

		if (exception != nullptr)
		{
			mono_print_unhandled_exception(exception);
		}
		else
		{
			ret = true;
		}
	}

	return ret;
}

CreatedMonoObject ModuleScripting::CreateMonoObject(MonoClass* mono_class)
{
	bool correct = false;

	MonoObject* obj = nullptr;
	uint id = 0;

	if (mono_class != nullptr)
	{
		obj = mono_object_new(base_domain, mono_class);

		if (obj != nullptr)
		{
			id = mono_gchandle_new(obj, true);

			correct = true;
		}
	}

	CreatedMonoObject ret(obj, id);
	ret.loaded = correct;

	return ret;
}

void ModuleScripting::DestroyMonoObject(CreatedMonoObject& created_mono_object)
{
	if (created_mono_object.loaded)
	{
		mono_gchandle_free(created_mono_object.GetId());

		created_mono_object.loaded = false;
	}
}

MonoString* ModuleScripting::MonoStringFromString(const char * str) const
{
	MonoString* ret = nullptr;

	ret = mono_string_new(base_domain, str);

	return ret;
}

MonoArray* ModuleScripting::MonoArrayFromVector(MonoClass* objects_mono_class, const std::vector<MonoObject*>& vec)
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

std::vector<MonoObject*> ModuleScripting::VectorFromMonoArray(MonoClass* objects_mono_class, MonoArray * mono_array)
{
	std::vector<MonoObject*> ret;

	if (objects_mono_class != nullptr && mono_array != nullptr)
	{
		uint count = MonoArrayCount(mono_array);

		for (int i = 0; i < count; ++i)
		{
			MonoObject* obj = mono_array_get(mono_array, MonoObject*, i);

			ret.push_back(obj);
		}
	}

	return ret;
}

bool ModuleScripting::BoolFromMonoBool(MonoBoolean * mono_bool)
{
	bool ret = false;

	if (mono_bool != nullptr)
	{
		ret = mono_bool;
	}

	return ret;
}

uint ModuleScripting::MonoArrayCount(MonoArray * mono_array) const
{
	uint ret = 0;

	if (mono_array != nullptr)
	{
		ret = mono_array_length(mono_array);
	}

	return ret;
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

MonoClass* ScriptingAssembly::GetClass(const char* class_namepsace, const char* class_name)
{
	MonoClass* ret = nullptr;

	if (loaded)
		ret = mono_class_from_name(image, class_namepsace, class_name);

	return ret;
}

CreatedMonoObject::CreatedMonoObject()
{
}

CreatedMonoObject::CreatedMonoObject(MonoObject * mono_objec, uint _id)
{
	mono_object = mono_objec;
	id = _id;
}

MonoObject * CreatedMonoObject::GetMonoObject() const
{
	return mono_object;
}

uint CreatedMonoObject::GetId() const
{
	return id;
}

bool CreatedMonoObject::GetLoaded() const
{
	return loaded;
}
