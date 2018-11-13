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

private:
	CreatedMonoObject script_compiler_object;
};

#endif // !__SCRIPTING_OBJECT_COMPILER_H__