#ifndef __SCRIPTING_BRIDGE_COMPONENT_CAMERA_H__
#define __SCRIPTING_BRIDGE_COMPONENT_CAMERA_H__

#include "ScriptingBridgeObject.h"
#include "ModuleScripting.h"

class GameObject;
class GameObjectComponent;
class Event;
class ComponentCamera;

class ScriptingBridgeComponentCamera: public ScriptingBridgeObject
{
	friend class ScriptingCluster;
	friend class ModuleScripting;
	friend class ComponentCamera;

private:
	void operator delete(void *) {}

public:
	ScriptingBridgeComponentCamera(ComponentCamera* component_ref);
	~ScriptingBridgeComponentCamera();

	void Start();
	void OnRebuildInstances();
	void CleanUp();

private:
	static ComponentCamera* GetComponentCameraFromMonoObject(MonoObject* mono_object);

	// Internal Calls


	// --------------

private:
	ComponentCamera* component_ref = nullptr;
};

#endif // !__SCRIPTING_BRIDGE_COMPONENT_CAMERA_H__