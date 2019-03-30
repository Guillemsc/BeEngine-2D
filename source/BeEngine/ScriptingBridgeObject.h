#ifndef __SCRIPTING_BRIDGE_OBJECT_H__
#define __SCRIPTING_BRIDGE_OBJECT_H__

#include "Globals.h"

class ScriptingClassInstance;
class ScriptingClass;

class ScriptingBridgeObject
{
	friend class ModuleScripting;

private:
	void operator delete(void *) {}

public:
	ScriptingBridgeObject(ScriptingClass* scripting_class);
	~ScriptingBridgeObject();

	void RebuildInstance();
	void DestroyInstance();
	ScriptingClassInstance* GetInstance() const;
	ScriptingClass* GetClass() const;

	virtual void Start() {};
	virtual void OnRebuildInstances() {};
	virtual void CleanUp() {};

private:
	void SetScriptingClass(ScriptingClass* scripting_class);

protected:
	ScriptingClass* scripting_class = nullptr;
	ScriptingClassInstance* class_instance = nullptr;
};

#endif // !__SCRIPTING_BRIDGE_OBJECT_H__