#ifndef __SCRIPTING_BRIDGE_COMPONENT_SPRITE_RENDERER_H__
#define __SCRIPTING_BRIDGE_COMPONENT_SPRITE_RENDERER_H__

#include "ScriptingBridgeObject.h"
#include "ModuleScripting.h"

class GameObject;
class GameObjectComponent;
class Event;
class ComponentSpriteRenderer;

class ScriptingBridgeComponentSpriteRenderer : public ScriptingBridgeObject
{
	friend class ScriptingCluster;
	friend class ModuleScripting;
	friend class ComponentSpriteRenderer;

private:
	void operator delete(void *) {}

public:
	ScriptingBridgeComponentSpriteRenderer(ComponentSpriteRenderer* component_ref);
	~ScriptingBridgeComponentSpriteRenderer();

	void Start();
	void OnRebuildInstances();
	void CleanUp();

private:

	// Internal Calls

	static void SetSpriteColour(MonoObject* mono_object, MonoObject* mono_object_colour);
	static MonoObject* GetSpriteColour(MonoObject* mono_object);

	// --------------

private:
	ComponentSpriteRenderer* component_ref = nullptr;
};

#endif // !__SCRIPTING_BRIDGE_COMPONENT_SPRITE_RENDERER_H__