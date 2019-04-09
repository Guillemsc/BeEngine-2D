#include "ScriptingBridgeComponentText.h"
#include "App.h"
#include "ModuleScripting.h"
#include "ScriptingCluster.h"
#include "ComponentText.h"
#include "GameObject.h"
#include "ScriptingBridgeBeObject.h"

ScriptingBridgeComponentText::ScriptingBridgeComponentText(ComponentText * component_text) :
	ScriptingBridgeObject(App->scripting->scripting_cluster->component_text_class)
{
	component_text_ref = component_text;
}

ScriptingBridgeComponentText::~ScriptingBridgeComponentText()
{
}

void ScriptingBridgeComponentText::Start()
{
}

void ScriptingBridgeComponentText::OnRebuildInstances()
{
	if (class_instance != nullptr)
	{
		ScriptingBridgeBeObject::SetBeObjectRefPointer(class_instance->GetMonoObject(), component_text_ref);

		MonoObject* owner_go_mono_object = component_text_ref->GetOwner()->GetScriptingBridge()->GetInstance()->GetMonoObject();

		void* args2[1] = { owner_go_mono_object };

		MonoObject* ret_obj2 = nullptr;
		class_instance->InvokeMonoMethodOnParentClass(
			*App->scripting->scripting_cluster->component_class, "SetOwner", args2, 1, ret_obj2);
	}
}

void ScriptingBridgeComponentText::CleanUp()
{
	if (class_instance != nullptr)
	{
		ScriptingBridgeBeObject::ClearBeObjectRefPointer(class_instance->GetMonoObject());
	}
}

void ScriptingBridgeComponentText::SetText(MonoObject * mono_object, MonoString * mono_string)
{
	ComponentText* comp = (ComponentText*)ScriptingBridgeBeObject::GetBeObjectRefPointer(mono_object);

	if (comp != nullptr)
	{
		std::string new_text = App->scripting->UnboxString(mono_string);

		comp->SetText(new_text);
	}
}

MonoString* ScriptingBridgeComponentText::GetText(MonoObject * mono_object)
{
	MonoString* ret = nullptr;

	ComponentText* comp = (ComponentText*)ScriptingBridgeBeObject::GetBeObjectRefPointer(mono_object);

	if (comp != nullptr)
	{
		ret = App->scripting->BoxString(comp->GetTextData().GetText().c_str());
	}

	return ret;
}

void ScriptingBridgeComponentText::SetTextColour(MonoObject * mono_object, MonoObject * mono_object_colour)
{
	ComponentText* comp = (ComponentText*)ScriptingBridgeBeObject::GetBeObjectRefPointer(mono_object);

	if (comp != nullptr)
	{
		float4 new_colour = App->scripting->scripting_cluster->UnboxColour(mono_object_colour);

		comp->SetColour(new_colour);
	}
}

MonoObject * ScriptingBridgeComponentText::GetTextColour(MonoObject * mono_object)
{
	MonoObject* ret = nullptr;

	ComponentText* comp = (ComponentText*)ScriptingBridgeBeObject::GetBeObjectRefPointer(mono_object);

	if (comp != nullptr)
	{
		ret = App->scripting->scripting_cluster->BoxColour(comp->GetColour());
	}

	return ret;
}
