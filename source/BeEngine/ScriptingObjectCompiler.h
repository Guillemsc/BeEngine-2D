#ifndef __SCRIPTING_OBJECT_COMPILER_H__
#define __SCRIPTING_OBJECT_COMPILER_H__

#include "Globals.h"
#include "ScriptingObject.h"
#include "ModuleScripting.h"

class ScriptingObjectCompiler : public ScriptingObject
{
	friend class ModuleScripting;

private:
	void operator delete(void *) {}

public:
	ScriptingObjectCompiler();
	~ScriptingObjectCompiler();

	void Start();
	void Update();
	void CleanUp();

	void SetScriptsAssemblyOutputFilepath(const char* set);

	bool AddScript(const char* script_filepath);
	bool RemoveScript(const char* script_filepath);
	bool CompileScripts(std::vector<std::string>& compile_errors = std::vector<std::string>());
	bool CreateScript(const char* script_filepath, const char* code);
	std::string GetScriptCode(const char* script_filepath);
	bool SetScriptCode(const char* script_filepath, std::string code);
	bool CreateScriptFromTemplate(const char* save_path, const char* name, std::string& created_asset_filepath = std::string());
	bool ClassIsSubclassOf(const char* class_to_check, const char* class_parent);

private:
	ScriptingClassInstance* script_compiler_instance = nullptr;

	std::string script_template_filepath;
	std::string scripts_assembly_output_filepath;
};

#endif // !__SCRIPTING_OBJECT_COMPILER_H__