#ifndef __SCRIPTING_BRIDGE_RESOURCE_AUDIO_H__
#define __SCRIPTING_BRIDGE_RESOURCE_AUDIO_H__

#include "ScriptingBridgeObject.h"
#include "ModuleScripting.h"

class Resource;
class ResourceAudio;
class Event;

class ScriptingBridgeResourceAudio : public ScriptingBridgeObject
{
	friend class ScriptingCluster;
	friend class ModuleScripting;
	friend class ResourceAudio;

private:
	void operator delete(void *) {}

public:
	ScriptingBridgeResourceAudio(ResourceAudio* resource_ref);
	~ScriptingBridgeResourceAudio();

	void Start();
	void OnRebuildInstances();
	void CleanUp();

private:

	// Internal Calls


	// --------------

private:
	ResourceAudio* resource_ref = nullptr;
};

#endif // !__SCRIPTING_BRIDGE_RESOURCE_AUDIO_H__