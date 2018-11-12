#ifndef __MODULE_SCRIPTING_H__
#define __MODULE_SCRIPTING_H__

#include "Module.h"
#include "Globals.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

#include "ScriptingObject.h"

class Event;
class ScriptingObjectCompiler;

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
	
	void OnEvent(Event* ev);

	ScriptingObject* AddScriptingObject(ScriptingObject* obj);
	void DestroyScriptingObject(ScriptingObject* obj);

	// Assembly management
	ScriptingAssembly* CreateAssembly(const char* assembly_path);
	void DestroyAssembly(ScriptingAssembly* assembly);
	std::vector<ScriptingAssembly*> GetScriptingAssemblys() const;

	// Script management
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

	// Box
	MonoString* MonoStringFromString(const char* str) const;
	MonoArray* MonoArrayFromVector(MonoClass* objects_mono_class, const std::vector<MonoObject*>& vec);

	// Unbox
	std::vector<MonoObject*> VectorFromMonoArray(MonoClass* objects_mono_class, MonoArray* mono_array);
	uint MonoArrayCount(MonoArray* mono_array) const;
	bool BoolFromMonoBool(MonoBoolean* mono_bool);

private:
	void DestroyAllScriptingObjects();
	void DestroyAllAssemblys();

public:
	ScriptingAssembly* scripting_assembly = nullptr;
	ScriptingAssembly* scripting_internal_assembly = nullptr;

	ScriptingObjectCompiler* compiler = nullptr;

private:
	std::string mono_base_path;
	std::string assembly_base_path;

	MonoDomain* base_domain = nullptr;

	std::vector<ScriptingAssembly*> assemblys;
	std::vector<ScriptingObject*> scripting_objects;
};

#endif // !__MODULE_SCRIPTING_H__