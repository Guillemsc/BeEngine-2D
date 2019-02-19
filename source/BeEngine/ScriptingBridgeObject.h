#ifndef __SCRIPTING_BRIDGE_OBJECT_H__
#define __SCRIPTING_BRIDGE_OBJECT_H__

#include "Globals.h"

class ScriptingBridgeObject
{
	friend class ModuleScripting;

private:
	void operator delete(void *) {}

public:
	ScriptingBridgeObject();
	~ScriptingBridgeObject();

	virtual void Start() {};
	virtual void RebuildInstances() {};
	virtual void CleanUp() {};

protected:
	bool loaded = false;
	bool ready_to_use = false;
};

#endif // !__SCRIPTING_BRIDGE_OBJECT_H__