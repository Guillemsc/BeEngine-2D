#include "ScriptingBridgeGameObject.h"
#include "App.h"
#include "GameObject.h"
#include "ModuleGameObject.h"
#include "GameObjectComponent.h"
#include "App.h"
#include "Globals.h"
#include "ScriptingCluster.h"

ScriptingBridgeGameObject::ScriptingBridgeGameObject(GameObject* go) : ScriptingBridgeObject()
{
	go_ref = go;
}

ScriptingBridgeGameObject::~ScriptingBridgeGameObject()
{
}

void ScriptingBridgeGameObject::Start()
{
	
}

void ScriptingBridgeGameObject::RebuildInstances()
{
	if (go_scripting_instance != nullptr)
	{
		go_scripting_instance->CleanUp();
		RELEASE(go_scripting_instance);
	}

	go_scripting_instance = App->scripting->scripting_cluster->game_object_class.CreateInstance();

	if (go_scripting_instance != nullptr)
	{
		MonoArray* mono_pointer = App->scripting->BoxPointer(go_ref);
	
		void* args[1] = { mono_pointer };
	
		MonoObject* ret_obj = nullptr;
		go_scripting_instance->InvokeMonoMethodOnParentClass(
			App->scripting->scripting_cluster->beengine_object_class, "SetPointerRef", args, 1, ret_obj);
	}
}

void ScriptingBridgeGameObject::CleanUp()
{
	if (go_scripting_instance != nullptr)
	{
		go_scripting_instance->CleanUp();
		RELEASE(go_scripting_instance);
	}
}

ScriptingClassInstance * ScriptingBridgeGameObject::GetGoScriptingInstance() const
{
	return go_scripting_instance;
}

GameObject* ScriptingBridgeGameObject::GetGameObjectFromMonoObject(MonoObject * mono_object)
{
	GameObject* ret = nullptr;

	if (mono_object != nullptr)
	{		
		MonoObject* obj_ret = nullptr;
		if (App->scripting->InvokeMonoMethod(mono_object, 
			App->scripting->scripting_cluster->beengine_object_class.GetMonoClass(), "GetPointerRef", nullptr, 0, obj_ret))
		{
			if (obj_ret != nullptr)
			{
				ret = (GameObject*)App->scripting->UnboxPointer((MonoArray*)obj_ret);
			}
		}
	}

	return ret;
}

GameObjectComponent * ScriptingBridgeGameObject::CreateGameObjectComponentFromComponentType(GameObject* go, MonoType * type)
{
	GameObjectComponent * ret = nullptr;

	if (go != nullptr)
	{
		MonoClass* type_class = App->scripting->UnboxType(type);

		if (type_class != nullptr)
		{
			ScriptingClass component_class(type_class);
		}
	}

	return ret;
}

void ScriptingBridgeGameObject::SetName(MonoObject * mono_object, MonoString * mono_string)
{
	GameObject* go = GetGameObjectFromMonoObject(mono_object);

	std::string new_name = App->scripting->UnboxString(mono_string);

	if (go != nullptr)
	{
		std::string new_name = App->scripting->UnboxString(mono_string);

		go->SetName(new_name.c_str());
	}
}

MonoString* ScriptingBridgeGameObject::GetName(MonoObject * mono_object)
{
	MonoString* ret = nullptr;

	GameObject* go = GetGameObjectFromMonoObject(mono_object);

	if (go != nullptr)
	{
		ret = App->scripting->BoxString(go->GetName().c_str());
	}

	return ret;
}

void ScriptingBridgeGameObject::AddComponent(MonoObject * mono_object, MonoType * component_type)
{
	GameObject* go = GetGameObjectFromMonoObject(mono_object);

	if (go != nullptr)
	{

	}
}
