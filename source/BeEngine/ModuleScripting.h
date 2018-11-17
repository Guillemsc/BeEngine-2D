#ifndef __MODULE_SCRIPTING_H__
#define __MODULE_SCRIPTING_H__

#include "Module.h"
#include "Globals.h"
#include "GeometryMath.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

#include "ScriptingObject.h"

class Event;
class ScriptingObjectCompiler;
class ScriptingObjectSolutionManager;
class ScriptingObjectFileWatcher;
class ScriptingClass;

class ScriptingAssembly
{
	friend class ModuleScripting;

public:
	ScriptingAssembly(MonoDomain* domain, const char* assembly_path);

	bool GetAssemblyLoaded() const;

	MonoAssembly* GetAssembly();
	MonoImage* GetImage();
	const char* GetPath();

	ScriptingClass GetClass(const char* class_namepsace, const char* class_name);

private:
	void LoadAssembly();

private:
	bool loaded = false;

	MonoDomain*   domain = nullptr;
	MonoAssembly* assembly = nullptr;
	MonoImage*    image = nullptr;

	std::string path;
};

class ScriptingClass
{
	friend class ScriptingClassInstance;

public:
	ScriptingClass();
	ScriptingClass(MonoClass* mono_class);

	const char* GetNamespace() const;

	bool InvokeStaticMonoMethod(const char* method_name, void **args, uint args_count, MonoObject*& return_object);

	ScriptingClassInstance* CreateInstance();

private:
	MonoClass* mono_class = nullptr;
};

class ScriptingClassInstance
{
	friend class ScriptingClass;

public:
	ScriptingClassInstance(ScriptingClass sci, MonoObject* mono_objec, uint id);

	void CleanUp();

	ScriptingClass GetClass();

	bool InvokeMonoMethod(const char* method_name, void **args, uint args_count, MonoObject*& return_object);
	bool InvokeMonoMethodUnmanaged(const char* method_name, void **args, uint args_count, void*& return_object);

private:
	ScriptingClass scripting_class;
	MonoObject* mono_object = nullptr;
	uint id = 0;
};

void MonoLogToLog(const char * log_domain, const char * log_level, const char * message, mono_bool fatal, void * user_data);
void MonoInternalWarning(const char * string, mono_bool is_stdout);
void MonoInternalError(const char * string, mono_bool is_stdout);

class ModuleScripting : public Module
{
	friend class ScriptingClass;

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
	MonoClass* GetMonoClass(MonoObject* obj);
	MonoType* GetMonoType(MonoClass* mono_class);
	const char* GetMonoTypeName(MonoType* mono_type);

	// Boxing
	MonoString* BoxString(const char* val);
	MonoObject* BoxBool(bool val);
	MonoObject* BoxInt(int val);
	MonoObject* BoxUint(uint val);
	MonoObject* BoxFloat(float val);
	MonoObject* BoxFloat2(const float2& val);
	MonoArray* BoxArray(MonoClass* objects_mono_class, const std::vector<MonoObject*>& vec);

	// Unboxing
	std::string UnboxString(MonoString* val);
	bool UnboxBool(MonoObject* val);
	int UnboxInt(MonoObject* val);
	uint UnboxUint(MonoObject* val);
	float UnboxFloat(MonoObject* val);
	float2 UnboxFloat2(MonoObject* val);
	std::vector<MonoObject*> UnboxArray(MonoClass* mono_class, MonoArray* val);
	uint UnboxArrayCount(MonoArray* val);

private:
	void InitScriptingSolution();
	void UpdateScriptingObjects();
	void DestroyAllAssemblys();
	void DestroyAllScriptingObjects();

public:
	ScriptingAssembly* scripting_assembly = nullptr;
	ScriptingAssembly* scripting_internal_assembly = nullptr;

	ScriptingObjectCompiler* compiler = nullptr;
	ScriptingObjectSolutionManager* solution_manager = nullptr;
	ScriptingObjectFileWatcher* file_watcher = nullptr;

private:
	std::string mono_base_path;
	std::string assembly_base_path;

	MonoDomain* base_domain = nullptr;
	
	std::vector<ScriptingObject*> scripting_objects;
	std::vector<ScriptingAssembly*> assemblys;
};

#endif // !__MODULE_SCRIPTING_H__