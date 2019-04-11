#ifndef __SCRIPTING_BRIDGE_RESOURCE_PREFAB_H__
#define __SCRIPTING_BRIDGE_RESOURCE_PREFAB_H__

#include "ScriptingBridgeObject.h"
#include "ModuleScripting.h"

class Resource;
class ResourcePrefab;
class Event;

class ScriptingBridgeResourcePrefab : public ScriptingBridgeObject
{
	friend class ScriptingCluster;
	friend class ModuleScripting;
	friend class ResourcePrefab;

private:
	void operator delete(void *) {}

public:
	ScriptingBridgeResourcePrefab(ResourcePrefab* resource_ref);
	~ScriptingBridgeResourcePrefab();

	void Start();
	void OnRebuildInstances();
	void CleanUp();

private:
	// Internal Calls


	// --------------

private:
	ResourcePrefab* resource_ref = nullptr;
};

#endif // !__SCRIPTING_BRIDGE_RESOURCE_PREFAB_H__