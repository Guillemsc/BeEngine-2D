#ifndef __SCRIPTING_BRIDGE_RESOURCE_FONT_H__
#define __SCRIPTING_BRIDGE_RESOURCE_FONT_H__

#include "ScriptingBridgeObject.h"
#include "ModuleScripting.h"

class Resource;
class ResourceFont;
class Event;

class ScriptingBridgeResourceFont : public ScriptingBridgeObject
{
	friend class ScriptingCluster;
	friend class ModuleScripting;
	friend class ResourceFont;

private:
	void operator delete(void *) {}

public:
	ScriptingBridgeResourceFont(ResourceFont* resource_ref);
	~ScriptingBridgeResourceFont();

	void Start();
	void OnRebuildInstances();
	void CleanUp();

private:
	// Internal Calls


	// --------------

private:
	ResourceFont* resource_ref = nullptr;
};

#endif // !__SCRIPTING_BRIDGE_RESOURCE_FONT_H__