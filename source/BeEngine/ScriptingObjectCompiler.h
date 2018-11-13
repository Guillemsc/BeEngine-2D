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

	bool CompileScript(const char* script_path, std::vector<std::string>& compile_errors = std::vector<std::string>());

private:
	ScriptingClassInstance* script_compiler_instance = nullptr;
};

#endif // !__SCRIPTING_OBJECT_COMPILER_H__