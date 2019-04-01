#include "ScriptingBridgeComponentCanvas.h"
#include "App.h"
#include "ComponentCanvas.h"
#include "ModuleScripting.h"
#include "ScriptingCluster.h"
#include "GameObject.h"

ScriptingBridgeComponentCanvas::ScriptingBridgeComponentCanvas(ComponentCanvas * component):
	ScriptingBridgeObject(App->scripting->scripting_cluster->component_canvas_class)
{
	component_ref = component;
}

ScriptingBridgeComponentCanvas::~ScriptingBridgeComponentCanvas()
{
}

void ScriptingBridgeComponentCanvas::Start()
{
}

void ScriptingBridgeComponentCanvas::OnRebuildInstances()
{
	if (class_instance != nullptr)
	{
		MonoArray* mono_pointer = App->scripting->BoxPointer(component_ref);

		void* args[1] = { mono_pointer };

		MonoObject* ret_obj = nullptr;
		class_instance->InvokeMonoMethodOnParentClass(
			*App->scripting->scripting_cluster->beengine_object_class, "SetPointerRef", args, 1, ret_obj);

		MonoObject* owner_go_mono_object = component_ref->GetOwner()->GetScriptingBridge()->GetInstance()->GetMonoObject();

		void* args2[1] = { owner_go_mono_object };

		MonoObject* ret_obj2 = nullptr;
		class_instance->InvokeMonoMethodOnParentClass(
			*App->scripting->scripting_cluster->component_class, "SetOwner", args2, 1, ret_obj2);
	}
}

void ScriptingBridgeComponentCanvas::CleanUp()
{
}

ComponentCanvas * ScriptingBridgeComponentCanvas::GetComponentCanvasFromMonoObject(MonoObject * mono_object)
{
	return nullptr;
}
