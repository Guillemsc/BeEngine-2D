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
	void CleanUp();

	void SetScriptTemplateFilepath(const char* set);

	bool CompileScript(const char* script_filepath, const char* dll_output_path, std::vector<std::string>& compile_errors = std::vector<std::string>());
	bool CreateScript(const char* script_filepath, const char* code);
	std::string GetScriptCode(const char* script_filepath);
	bool SetScriptCode(const char* script_filepath, std::string code);
	bool CreateScriptFromTemplate(const char* save_path, const char* name, std::string& created_asset_filepath = std::string());

private:
	ScriptingClassInstance* script_compiler_instance = nullptr;

	std::string script_template_filepath;
};

#endif // !__SCRIPTING_OBJECT_COMPILER_H__