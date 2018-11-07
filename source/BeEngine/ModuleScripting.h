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
	const char* GetPath();

	MonoClass* GetClass(const char* class_namepsace, const char* class_name);

private:
	bool loaded = false;

	MonoDomain*   domain = nullptr;
	MonoAssembly* assembly = nullptr;
	MonoImage*    image = nullptr;

	std::string path;
};

class CreatedMonoObject
{
	friend class ModuleScripting;

public:
	CreatedMonoObject(MonoObject* mono_objec, uint id);

	MonoObject* GetMonoObject() const;
	uint GetId() const;

	bool GetLoaded() const;

private:
	CreatedMonoObject();

private:
	MonoObject* mono_object = nullptr;
	uint id = 0;
	bool loaded = false;
};

class TestMono : public MonoObject
{
public:
	TestMono() {};

	int i = 2;
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

	bool InitCompiler();

	ScriptingAssembly* CreateAssembly(const char* assembly_path);
	void DestroyAssembly(ScriptingAssembly* assembly);

	bool CompileScript(const char* filepath, std::string& compile_errors = std::string());

	MonoClass* GetMonoClass(ScriptingAssembly* assembly, const char* class_namepsace, const char* class_name);
	MonoClass* GetMonoClass(MonoObject* obj);
	const char* GetMonoClassNamespace(MonoClass* mono_class);
	MonoType* GetMonoType(MonoClass* mono_class);
	const char* GetMonoTypeName(MonoType* mono_type);
	MonoMethod* GetMonoMethod(MonoClass* mono_class, const char* method_name, uint args_count);
	bool InvokeStaticMonoMethod(MonoMethod* mono_method, void **args, MonoObject*& return_object);
	bool InvokeMonoMethod(MonoObject* obj, MonoMethod* mono_method, void **args, MonoObject*& return_object);
	bool InvokeMonoMethodUnmanaged(MonoObject* obj, MonoMethod* mono_method, void **args, void*& return_object);
	CreatedMonoObject CreateMonoObject(MonoClass* mono_class);
	void DestroyMonoObject(CreatedMonoObject& created_mono_object);

	MonoString* MonoStringFromString(const char* str) const;
	MonoArray* MonoArrayFromVector(MonoClass* objects_mono_class, const std::vector<MonoObject*>& vec);
	std::vector<MonoObject*> VectorFromMonoArray(MonoClass* objects_mono_class, MonoArray* mono_array);
	bool BoolFromMonoBool(MonoBoolean* mono_bool);
	uint MonoArrayCount(MonoArray* mono_array) const;


private:
	std::string mono_base_path;
	std::string assembly_base_path;

	MonoDomain* base_domain = nullptr;

	std::vector<ScriptingAssembly*> assemblys;

	ScriptingAssembly* base_project_assembly = nullptr;
	ScriptingAssembly* compiler_assembly = nullptr;

	CreatedMonoObject  compiler_object;
};

#endif // !__MODULE_SCRIPTING_H__