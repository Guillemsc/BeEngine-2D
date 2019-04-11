#ifndef __SCRIPTING_BRIDGE_RESOURCE_SCENE_H__
#define __SCRIPTING_BRIDGE_RESOURCE_SCENE_H__

#include "ScriptingBridgeObject.h"
#include "ModuleScripting.h"

class Resource;
class ResourceScene;
class Event;

class ScriptingBridgeResourceScene : public ScriptingBridgeObject
{
	friend class ScriptingCluster;
	friend class ModuleScripting;
	friend class ResourceScene;

private:
	void operator delete(void *) {}

public:
	ScriptingBridgeResourceScene(ResourceScene* resource_ref);
	~ScriptingBridgeResourceScene();

	void Start();
	void OnRebuildInstances();
	void CleanUp();

private:
	// Internal Calls


	// --------------

private:
	ResourceScene* resource_ref = nullptr;
};

#endif // !__SCRIPTING_BRIDGE_RESOURCE_SCENE_H__