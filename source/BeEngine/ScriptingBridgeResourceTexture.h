#ifndef __SCRIPTING_BRIDGE_RESOURCE_TEXTURE_H__
#define __SCRIPTING_BRIDGE_RESOURCE_TEXTURE_H__

#include "ScriptingBridgeObject.h"
#include "ModuleScripting.h"

class Resource;
class ResourceTexture;
class Event;

class ScriptingBridgeResourceTexture : public ScriptingBridgeObject
{
	friend class ScriptingCluster;
	friend class ModuleScripting;
	friend class ResourceTexture;

private:
	void operator delete(void *) {}

public:
	ScriptingBridgeResourceTexture(ResourceTexture* resource_ref);
	~ScriptingBridgeResourceTexture();

	void Start();
	void OnRebuildInstances();
	void CleanUp();

private:
	// Internal Calls


	// --------------

private:
	ResourceTexture* resource_ref = nullptr;
};

#endif // !__SCRIPTING_BRIDGE_RESOURCE_TEXTURE_H__