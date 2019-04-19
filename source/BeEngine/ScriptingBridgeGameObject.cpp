#include "ScriptingBridgeGameObject.h"
#include "App.h"
#include "GameObject.h"
#include "ModuleGameObject.h"
#include "GameObjectComponent.h"
#include "App.h"
#include "Globals.h"
#include "ScriptingCluster.h"
#include "ComponentTransfrom.h"
#include "ScriptingBridgeComponentTransform.h"
#include "ComponentButton.h"
#include "ScriptingBridgeComponentButton.h"
#include "ScriptingBridgeBeObject.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

ScriptingBridgeGameObject::ScriptingBridgeGameObject(GameObject* go) 
	: ScriptingBridgeObject(App->scripting->scripting_cluster->game_object_class)
{
	go_ref = go;
}

ScriptingBridgeGameObject::~ScriptingBridgeGameObject()
{
}

void ScriptingBridgeGameObject::Start()
{
	
}

void ScriptingBridgeGameObject::OnRebuildInstances()
{
	if (class_instance != nullptr)
	{
		ScriptingBridgeBeObject::SetBeObjectRefPointer(class_instance->GetMonoObject(), go_ref);
	}
}

void ScriptingBridgeGameObject::CleanUp()
{
	if (class_instance != nullptr)
	{
		ScriptingBridgeBeObject::ClearBeObjectRefPointer(class_instance->GetMonoObject());
	}
}

void ScriptingBridgeGameObject::SetComponentTransform(ComponentTransform * trans)
{
	if (class_instance != nullptr)
	{
		if (trans != nullptr)
		{
			ScriptingClassInstance* scripting_instance_transform = trans->GetScriptingBridge()->GetInstance();

			if (scripting_instance_transform != nullptr)
			{
				MonoObject* mono_object_transform = scripting_instance_transform->GetMonoObject();

				void* args[1] = { mono_object_transform };

				MonoObject* ret_obj = nullptr;
				class_instance->InvokeMonoMethodOnParentClass(
					*App->scripting->scripting_cluster->game_object_class, "SetComponentTransform", args, 1, ret_obj);
			}
		}
	}
}

void ScriptingBridgeGameObject::CallOnCollisionEnter(PhysicsBody * pb)
{
	if (class_instance != nullptr)
	{
		if (pb != nullptr)
		{
			MonoObject* collision_mono_obj = App->scripting->scripting_cluster->BoxCollision(pb);

			void* args[1] = { collision_mono_obj };

			MonoObject* ret_obj = nullptr;
			class_instance->InvokeMonoMethod("CallOnCollisionEnter", args, 1, ret_obj);
		}
	}
}

void ScriptingBridgeGameObject::CallOnCollisionStay(PhysicsBody * pb)
{
	if (class_instance != nullptr)
	{
		if (pb != nullptr)
		{
			MonoObject* collision_mono_obj = App->scripting->scripting_cluster->BoxCollision(pb);

			void* args[1] = { collision_mono_obj };

			MonoObject* ret_obj = nullptr;
			class_instance->InvokeMonoMethod("CallOnCollisionStay", args, 1, ret_obj);
		}
	}
}

void ScriptingBridgeGameObject::CallOnCollisionExit(PhysicsBody * pb)
{
	if (class_instance != nullptr)
	{
		if (pb != nullptr)
		{
			MonoObject* collision_mono_obj = App->scripting->scripting_cluster->BoxCollision(pb);

			void* args[1] = { collision_mono_obj };

			MonoObject* ret_obj = nullptr;
			class_instance->InvokeMonoMethod("CallOnCollisionExit", args, 1, ret_obj);
		}
	}
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
	GameObject* go = (GameObject*)ScriptingBridgeBeObject::GetBeObjectRefPointer(mono_object);

	if (go != nullptr)
	{
		std::string new_name = App->scripting->UnboxString(mono_string);

		go->SetName(new_name.c_str());
	}
}

MonoString* ScriptingBridgeGameObject::GetName(MonoObject * mono_object)
{
	MonoString* ret = nullptr;

	GameObject* go = (GameObject*)ScriptingBridgeBeObject::GetBeObjectRefPointer(mono_object);

	if (go != nullptr)
	{
		ret = App->scripting->BoxString(go->GetName().c_str());
	}

	return ret;
}

void ScriptingBridgeGameObject::SetActive(MonoObject * mono_object, void* mono_value)
{
	GameObject* go = (GameObject*)ScriptingBridgeBeObject::GetBeObjectRefPointer(mono_object);

	if (go != nullptr)
	{
		bool new_val = mono_value;

		go->SetActive(new_val);
	}
}

void* ScriptingBridgeGameObject::GetActive(MonoObject * mono_object)
{
	void* ret = nullptr;

	GameObject* go = (GameObject*)ScriptingBridgeBeObject::GetBeObjectRefPointer(mono_object);

	if (go != nullptr)
	{
		bool active = go->GetActive();

		ret = &active;
	}

	return ret;
}

MonoObject* ScriptingBridgeGameObject::AddComponent(MonoObject * mono_object, MonoString * component_type)
{
	MonoObject* ret = nullptr;

	GameObject* go = (GameObject*)ScriptingBridgeBeObject::GetBeObjectRefPointer(mono_object);

	if (go != nullptr)
	{
		std::string type_name = App->scripting->UnboxString(component_type);

		ComponentType type = App->gameobject->GetComponentTypeByComponentScriptingName(type_name);
		
		GameObjectComponent* comp = nullptr;

		if (type != ComponentType::COMPONENT_TYPE_UNDEFINED)
		{
			comp = go->CreateComponent(type);
		}
		else
		{
			comp = (GameObjectComponent*)go->CreateComponentScript(type_name);
		}

		if (comp != nullptr)
		{
			ScriptingClassInstance* ins = comp->GetScriptingBridge()->GetInstance();

			if (ins != nullptr)
				ret = ins->GetMonoObject();
		}

		if (ret == nullptr)
			CONSOLE_ERROR("Component %s could not be created, as it does not exist on Bridge Code", type_name.c_str());
	}

	return ret;
}

MonoObject* ScriptingBridgeGameObject::GetComponent(MonoObject * mono_object, MonoString * component_type, int index)
{
	MonoObject* ret = nullptr;

	GameObject* go = (GameObject*)ScriptingBridgeBeObject::GetBeObjectRefPointer(mono_object);

	if (go != nullptr)
	{
		std::string type_name = App->scripting->UnboxString(component_type);

		ComponentType type = App->gameobject->GetComponentTypeByComponentScriptingName(type_name);

		GameObjectComponent* comp = nullptr;
		
		if (type != ComponentType::COMPONENT_TYPE_UNDEFINED)
		{
			comp = go->GetComponent(type, index);
		}
		else
		{
			comp = (GameObjectComponent*)go->GetComponentScript(type_name, index);
		}
		
		if (comp != nullptr)
		{
			ScriptingClassInstance* ins = comp->GetScriptingBridge()->GetInstance();

			if (ins != nullptr)
				ret = ins->GetMonoObject();
		}
	}

	return ret;
}
