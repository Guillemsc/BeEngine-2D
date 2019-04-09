#ifndef __SCRIPTING_BRIDGE_COMPONENT_CANVAS_H__
#define __SCRIPTING_BRIDGE_COMPONENT_CANVAS_H__

#include "ScriptingBridgeObject.h"
#include "ModuleScripting.h"

class GameObject;
class GameObjectComponent;
class Event;
class ComponentCanvas;

class ScriptingBridgeComponentCanvas : public ScriptingBridgeObject
{
	friend class ScriptingCluster;
	friend class ModuleScripting;
	friend class ComponentCanvas;

private:
	void operator delete(void *) {}

public:
	ScriptingBridgeComponentCanvas(ComponentCanvas* component_ref);
	~ScriptingBridgeComponentCanvas();

	void Start();
	void OnRebuildInstances();
	void CleanUp();

private:
	// Internal Calls


	// --------------

private:
	ComponentCanvas* component_ref = nullptr;
};

#endif // !__SCRIPTING_BRIDGE_COMPONENT_CANVAS_H__