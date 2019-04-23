#ifndef __SCRIPTING_BRIDGE_APPLICATION_H__
#define __SCRIPTING_BRIDGE_APPLICATION_H__

#include "ScriptingBridgeObject.h"
#include "ModuleScripting.h"

class ScriptingBridgeApplication : public ScriptingBridgeObject
{
	friend class ScriptingCluster;
	friend class ModuleScripting;
	friend class GameObject;

private:
	void operator delete(void *) {}

public:
	ScriptingBridgeApplication();
	~ScriptingBridgeApplication();

	void Start();
	void RebuildInstances();
	void PostRebuildInstances();
	void CleanUp();

private:

	// Internal Calls

	static void ApplicationQuit();

	// --------------
};

#endif // !__SCRIPTING_BRIDGE_APPLICATION_H__