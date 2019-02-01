#ifndef __MODULE_SCRIPTING_H__
#define __MODULE_SCRIPTING_H__

#include "Module.h"
#include "Globals.h"
#include "GeometryMath.h"
#include "Timer.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

#include <map>

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
	ScriptingAssembly(const char* assembly_path);

	void CleanUp();

	bool GetAssemblyLoaded() const;

	MonoAssembly* GetAssembly();
	MonoImage* GetImage();
	const char* GetPath();

	bool GetClass(const char* class_namepsace, const char* class_name, ScriptingClass& class_returned);

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

	std::string GetNamespace() const;
	std::string GetName() const;

	bool GetParentClass(ScriptingClass& returned_parent_class);
	bool GetIsInheritedFrom(const ScriptingClass& class_parent);

	std::map<std::string, MonoType*> GetFields();

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
	friend class ScriptingAssembly;

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
	std::vector<ScriptingAssembly*> GetScriptingAssemblys() const;

	// C# Base libs
	std::vector<std::string> GetBaseLibs() const;

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

	// Compiling
	void CompileScripts();
	void ForceCompileScripts();
	bool GetScriptsCompile() const;

private:
	void LoadDomain();
	void UnloadDomain();

	void CreateBaseDomainAndAssemblys();
	void DestroyBaseDomainAndAssemblys();

	void InitScriptingSolution();

	void ManageScriptsToCompile();

	void UpdateScriptingObjects();

	void DestroyAllAssemblys();
	void DestroyAllScriptingObjects();

public:
	ScriptingAssembly* scripting_assembly = nullptr;
	ScriptingAssembly* scripting_internal_assembly = nullptr;
	ScriptingAssembly* user_code_assembly = nullptr;

	ScriptingObjectCompiler* compiler = nullptr;
	ScriptingObjectSolutionManager* solution_manager = nullptr;
	ScriptingObjectFileWatcher* file_watcher = nullptr;

private:
	bool has_active_domain = false;

	std::string mono_base_path;
	std::string assembly_base_path;
	
	std::vector<ScriptingObject*> scripting_objects;

	std::vector<ScriptingAssembly*> assemblys;
	std::vector<std::string> base_libs;

	std::string scripting_user_assembly_filepath;

	bool needs_to_compile_user_scripts = false;
	Timer compile_user_scripts_timer;
	bool force_compile_scripts = false;
	bool user_code_compiles = true;
};

#endif // !__MODULE_SCRIPTING_H__