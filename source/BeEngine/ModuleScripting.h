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
#include "ScriptingBridgeObject.h"

class Event;
class ScriptingObjectCompiler;
class ScriptingObjectSolutionManager;
class ScriptingObjectFileWatcher;
class ScriptingClass;
class ScriptingItemGameObject;
class ScriptingItemComponent;
class ScriptingCluster;

class ScriptingAssembly
{
	friend class ModuleScripting;

public:
	ScriptingAssembly(const char* assembly_path, bool used_to_compile = true);

	void CleanUp();

	bool GetAssemblyLoaded() const;

	MonoAssembly* GetAssembly();
	MonoImage* GetImage();
	const char* GetPath();

	bool GetClass(const char* class_namepsace, const char* class_name, ScriptingClass& class_returned);

	bool GetUsedToCompile() const;

private:
	void LoadAssembly();

private:
	bool loaded = false;

	MonoDomain*   domain = nullptr;
	MonoAssembly* assembly = nullptr;
	MonoImage*    image = nullptr;

	std::string path;

	bool used_to_compile = true;
};

class ScriptingClass
{
	friend class ScriptingClassInstance;

public:
	ScriptingClass();
	ScriptingClass(MonoClass* mono_class);
	ScriptingClass(const ScriptingClass& scripting_class);

	bool GetLoaded() const;

	std::string GetNamespace() const;
	std::string GetName() const;
	MonoClass* GetMonoClass() const;

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
	ScriptingClassInstance(ScriptingClass sci, uint id);

	void CleanUp();

	ScriptingClass GetClass();

	bool InvokeMonoMethod(const char* method_name, void **args, uint args_count, MonoObject*& return_object);
	bool InvokeMonoMethodUnmanaged(const char* method_name, void **args, uint args_count, void*& return_object);

	bool InvokeMonoMethodOnParentClass(ScriptingClass parent_mono_class, const char* method_name, void **args, uint args_count, MonoObject*& return_object);
	bool InvokeMonoMethodUnmanagedOnParentClass(ScriptingClass parent_mono_class, const char* method_name, void **args, uint args_count, void*& return_object);

	MonoObject* GetMonoObject() const;

private:
	ScriptingClass scripting_class;
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

	bool GetUserCodeCompiles() const;

	ScriptingObject* AddScriptingObject(ScriptingObject* obj);
	void DestroyScriptingObject(ScriptingObject* obj);

	void AddScriptingBridgeObject(ScriptingBridgeObject* obj);
	void ScriptingBridgeRebuildInstances(ScriptingBridgeObject* obj);
	void DestroyScriptingBridgeObject(ScriptingBridgeObject* obj);

	// Assembly management
	ScriptingAssembly* CreateAssembly(const char* assembly_path, bool used_to_compìle = true);
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
	MonoObject* BoxChar(char val);
	MonoArray* BoxArray(MonoClass* objects_mono_class, const std::vector<MonoObject*>& vec);
	MonoArray* BoxBuffer(const char* buffer, uint buffer_size);
	MonoArray* BoxPointer(void* pointer);
	MonoType* BoxType(MonoClass* val);

	// Unboxing
	std::string UnboxString(MonoString* val);
	bool UnboxBool(MonoObject* val);
	int UnboxInt(MonoObject* val);
	uint UnboxUint(MonoObject* val);
	float UnboxFloat(MonoObject* val);
	float2 UnboxFloat2(MonoObject* val);
	char UnboxChar(MonoObject* val);
	std::vector<MonoObject*> UnboxArray(MonoArray* val);
	uint UnboxArrayCount(MonoArray* val);
	char* UnboxBuffer(MonoArray* val, uint& buffer_size);
	void* UnboxPointer(MonoArray* val);
	MonoClass* UnboxType(MonoType* val);

	bool InvokeMonoMethod(MonoObject* mono_object_ins, MonoClass* mono_class, const char* method_name,
		void **args, uint args_count, MonoObject*& return_object);

	// Compiling
	void CompileScripts();
	void ForceCompileScripts();
	bool GetScriptsCompile() const;
	bool GetNeedsToCompileScripts() const;

private:
	void LoadDomain();
	void UnloadDomain();

	void CreateBaseDomainAndAssemblys();
	void DestroyBaseDomainAndAssemblys();

	void InitScriptingSolution();

	void ManageScriptsToCompile();

	void UpdateScriptingObjects();
	void RebuildScriptingBridgeObjects();

	void DestroyAllAssemblys();
	void DestroyAllScriptingObjects();

public:
	ScriptingCluster* scripting_cluster = nullptr;

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

	MonoDomain* mono_root_domain = nullptr;
	
	std::vector<ScriptingObject*> scripting_objects;
	std::vector<ScriptingBridgeObject*> scripting_bridge_objects;

	std::vector<ScriptingAssembly*> assemblys;
	std::vector<std::string> base_libs;

	std::string scripting_user_assembly_filepath;

	bool needs_to_compile_user_scripts = false;
	Timer compile_user_scripts_timer;
	bool force_compile_scripts = false;
	bool user_code_compiles = false;
};

#endif // !__MODULE_SCRIPTING_H__