#include "ScriptingBridgeObject.h"
#include "ModuleScripting.h"
#include "App.h"
#include "ScriptingCluster.h"

ScriptingBridgeObject::ScriptingBridgeObject(ScriptingClass * set_scripting_class)
{
	SetScriptingClass(set_scripting_class);
}

ScriptingBridgeObject::~ScriptingBridgeObject()
{
}

void ScriptingBridgeObject::SetScriptingClass(ScriptingClass * set_scripting_class)
{
	scripting_class = set_scripting_class;

	RebuildInstance();
}

void ScriptingBridgeObject::RebuildInstance()
{
	DestroyInstance();

	if (scripting_class != nullptr)
		class_instance = scripting_class->CreateInstance();
}

void ScriptingBridgeObject::DestroyInstance()
{
	if (class_instance != nullptr)
	{
		class_instance->DestroyReference();
		RELEASE(class_instance);
	}
}

ScriptingClassInstance* ScriptingBridgeObject::GetInstance() const
{
	return class_instance;
}

ScriptingClass* ScriptingBridgeObject::GetClass() const
{
	return scripting_class;
}
