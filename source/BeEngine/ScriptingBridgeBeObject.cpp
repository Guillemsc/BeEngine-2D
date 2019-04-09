#include "ScriptingBridgeBeObject.h"
#include "App.h"
#include "ModuleScripting.h"
#include "ScriptingCluster.h"

ScriptingBridgeBeObject::ScriptingBridgeBeObject()
	: ScriptingBridgeObject(nullptr)
{
}

ScriptingBridgeBeObject::~ScriptingBridgeBeObject()
{
}

void ScriptingBridgeBeObject::Start()
{
}

void ScriptingBridgeBeObject::RebuildInstances()
{
}

void ScriptingBridgeBeObject::PostRebuildInstances()
{
}

void ScriptingBridgeBeObject::CleanUp()
{
}

bool ScriptingBridgeBeObject::SetBeObjectRefPointer(MonoObject * obj, void * pointer)
{
	bool ret = false;

	if (obj != nullptr)
	{
		MonoString* mono_pointer = App->scripting->BoxPointer(pointer);

		void* args[1] = { mono_pointer };

		MonoObject* obj_ret = nullptr;
		if (App->scripting->InvokeMonoMethod(obj,
			App->scripting->scripting_cluster->beengine_object_class->GetMonoClass(), "SetPointerRef", args, 1, obj_ret))
		{
			ret = true;
		}
	}

	return ret;
}

void* ScriptingBridgeBeObject::GetBeObjectRefPointer(MonoObject * obj)
{
	void* ret = nullptr;

	if (obj != nullptr)
	{
		bool valid = GetBeObjectRefValid(obj);

		if (valid)
		{
			MonoObject* obj_ret = nullptr;
			if (App->scripting->InvokeMonoMethod(obj,
				App->scripting->scripting_cluster->beengine_object_class->GetMonoClass(), "GetPointerRef", nullptr, 0, obj_ret))
			{
				if (obj_ret != nullptr)
				{
					ret = App->scripting->UnboxPointer((MonoString*)obj_ret);
				}
			}
		}
	}

	return ret;
}

void ScriptingBridgeBeObject::ClearBeObjectRefPointer(MonoObject * obj)
{
	if (obj != nullptr)
	{
		MonoObject* obj_ret = nullptr;
		App->scripting->InvokeMonoMethod(obj,
			App->scripting->scripting_cluster->beengine_object_class->GetMonoClass(), "ClearPointerRef", nullptr, 0, obj_ret);
	}
}

bool ScriptingBridgeBeObject::GetBeObjectRefValid(MonoObject * obj)
{
	bool ret = false;

	if (obj != nullptr)
	{
		MonoObject* obj_ret = nullptr;
		if (App->scripting->InvokeMonoMethod(obj,
			App->scripting->scripting_cluster->beengine_object_class->GetMonoClass(), "GetPointerRefValid", nullptr, 0, obj_ret))
		{
			if (obj_ret != nullptr)
			{
				ret = App->scripting->UnboxBool(obj_ret);
			}
		}
	}

	return ret;
}
