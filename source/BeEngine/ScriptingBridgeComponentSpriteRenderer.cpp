#include "ScriptingBridgeComponentSpriteRenderer.h"
#include "App.h"
#include "ModuleScripting.h"
#include "ScriptingCluster.h"
#include "ComponentSpriteRenderer.h"
#include "GameObject.h"
#include "ScriptingBridgeBeObject.h"

ScriptingBridgeComponentSpriteRenderer::ScriptingBridgeComponentSpriteRenderer(ComponentSpriteRenderer * component) : 
	ScriptingBridgeObject(App->scripting->scripting_cluster->component_sprite_renderer_class)
{
	component_ref = component;
}

ScriptingBridgeComponentSpriteRenderer::~ScriptingBridgeComponentSpriteRenderer()
{
}

void ScriptingBridgeComponentSpriteRenderer::Start()
{
}

void ScriptingBridgeComponentSpriteRenderer::OnRebuildInstances()
{
	if (class_instance != nullptr)
	{
		ScriptingBridgeBeObject::SetBeObjectRefPointer(class_instance->GetMonoObject(), component_ref);

		MonoObject* owner_go_mono_object = component_ref->GetOwner()->GetScriptingBridge()->GetInstance()->GetMonoObject();

		void* args2[1] = { owner_go_mono_object };

		MonoObject* ret_obj2 = nullptr;
		class_instance->InvokeMonoMethodOnParentClass(
			*App->scripting->scripting_cluster->component_class, "SetOwner", args2, 1, ret_obj2);
	}
}

void ScriptingBridgeComponentSpriteRenderer::CleanUp()
{
	if (class_instance != nullptr)
	{
		ScriptingBridgeBeObject::ClearBeObjectRefPointer(class_instance->GetMonoObject());
	}
}

void ScriptingBridgeComponentSpriteRenderer::SetSpriteColour(MonoObject * mono_object, MonoObject * mono_object_colour)
{
	ComponentSpriteRenderer* comp = (ComponentSpriteRenderer*)ScriptingBridgeBeObject::GetBeObjectRefPointer(mono_object);

	if (comp != nullptr)
	{
		float4 new_colour = App->scripting->scripting_cluster->UnboxColour(mono_object_colour);

		comp->SetColour(new_colour);
	}
}

MonoObject * ScriptingBridgeComponentSpriteRenderer::GetSpriteColour(MonoObject * mono_object)
{
	MonoObject* ret = nullptr;

	ComponentSpriteRenderer* comp = (ComponentSpriteRenderer*)ScriptingBridgeBeObject::GetBeObjectRefPointer(mono_object);

	if (comp != nullptr)
	{
		ret = App->scripting->scripting_cluster->BoxColour(comp->GetColour());
	}

	return ret;
}
