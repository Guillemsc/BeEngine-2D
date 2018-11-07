#ifndef __MODULE_SCRIPTING_H__
#define __MODULE_SCRIPTING_H__

#include "Module.h"
#include "Globals.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

class ScriptingAssembly
{
public:
	ScriptingAssembly(MonoDomain* domain, const char* assembly_path);

	void LoadAssembly();
	bool GetAssemblyLoaded() const;

	MonoAssembly* GetAssembly();
	MonoImage* GetImage();

	MonoClass* GetClass(const char* class_namepsace, const char* class_name);

private:
	bool loaded = false;

	MonoDomain*   domain = nullptr;
	MonoAssembly* assembly = nullptr;
	MonoImage*    image = nullptr;

	std::string path;
};

class ScriptingArgs
{
public:
	ScriptingArgs();


};

class ModuleScripting : public Module
{
public:
	ModuleScripting();
	~ModuleScripting();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

	ScriptingAssembly* CreateAssembly(const char* assembly_path);
	void DestroyAssembly(ScriptingAssembly* assembly);

	bool CompileScript(const char* filepath, std::string& compile_errors = std::string());

	const char* GetClassNamespace(MonoClass* mono_class);
	MonoMethod* GetMethod(MonoClass* mono_class, const char* method_name, uint args_count);
	bool InvokeMethod(MonoMethod* mono_method, void **args, MonoObject*& return_object);

	MonoString* CreateString(const char* str) const;

private:
	std::string mono_base_path;
	std::string assembly_base_path;

	MonoDomain* base_domain = nullptr;

	std::vector<ScriptingAssembly*> assemblys;

	ScriptingAssembly* compiler_assembly = nullptr;
};

#endif // !__MODULE_SCRIPTING_H__