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

	CreateAssembly(assembly_base_project_path.c_str());
	compiler_assembly = CreateAssembly(assembly_compiler_path.c_str());

	if (compiler_assembly == nullptr)
	{
		INTERNAL_LOG("Mono compiler assembly not loaded");

		ret = false;
	}

	CompileScript("asd");

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

	mono_jit_cleanup(base_domain);

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

		MonoMethod* compile_method = GetMethod(compiler_class, "CompileScript", 3);

		void* args[3];
		const char* script_path = "";
		const char* script_name = "";
		
		//args[0] = &value;

		MonoObject* return_values = nullptr;
		if (InvokeMethod(compile_method, nullptr, return_values))
		{

		}

		//string script_path, string script_name, ref List<string> compile_errors
	}

	return false;
}

const char * ModuleScripting::GetClassNamespace(MonoClass * mono_class)
{
	std::string ret = "";

	if (mono_class != nullptr)
		ret = mono_class_get_namespace(mono_class);

	return ret.c_str();
}

MonoMethod * ModuleScripting::GetMethod(MonoClass * mono_class, const char * method_name, uint args_count)
{
	MonoMethod* ret = nullptr;

	if (mono_class != nullptr)
		ret = mono_class_get_method_from_name(mono_class, method_name, args_count);

	return ret;
}

bool ModuleScripting::InvokeMethod(MonoMethod * mono_method, void ** args, MonoObject *& return_object)
{
	bool ret = false;

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

	return ret;
}

MonoString* ModuleScripting::CreateString(const char * str) const
{
	MonoString* ret = nullptr;

	ret = mono_string_new(base_domain, str);

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

MonoClass* ScriptingAssembly::GetClass(const char* class_namepsace, const char* class_name)
{
	MonoClass* ret = nullptr;

	if (loaded)
		ret = mono_class_from_name(image, class_namepsace, class_name);

	return ret;
}
