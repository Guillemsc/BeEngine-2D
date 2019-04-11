#ifndef __SCRIPTING_BRIDGE_RESOURCE_SCRIPT_H__
#define __SCRIPTING_BRIDGE_RESOURCE_SCRIPT_H__

#include "ScriptingBridgeObject.h"
#include "ModuleScripting.h"

class Resource;
class ResourceScript;
class Event;

class ScriptingBridgeResourceScript : public ScriptingBridgeObject
{
	friend class ScriptingCluster;
	friend class ModuleScripting;
	friend class ResourceScript;

private:
	void operator delete(void *) {}

public:
	ScriptingBridgeResourceScript(ResourceScript* resource_ref);
	~ScriptingBridgeResourceScript();

	void Start();
	void OnRebuildInstances();
	void CleanUp();

private:
	// Internal Calls


	// --------------

private:
	ResourceScript* resource_ref = nullptr;
};

#endif // !__SCRIPTING_BRIDGE_RESOURCE_SCRIPT_H__