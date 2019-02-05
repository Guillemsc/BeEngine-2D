#ifndef __SCRIPTING_ITEM_H__
#define __SCRIPTING_ITEM_H__

#include "Globals.h"

class ScriptingClassInstance;

class ScriptingItem
{
	friend class ModuleScripting;

private:
	void operator delete(void *) {}

public:
	ScriptingItem();
	~ScriptingItem();

	virtual void Start() {};
	virtual void RegisterInternalCalls() {};
	virtual void RebuildClasses() {};
	virtual void RebuildInstances() {};
	virtual void CleanUp() {};

protected:
	bool loaded = false;
};

#endif // !__SCRIPTING_ITEM_H__