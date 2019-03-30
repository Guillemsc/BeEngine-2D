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
		MonoArray* mono_pointer = App->scripting->BoxPointer(go_ref);
	
		void* args[1] = { mono_pointer };
	
		MonoObject* ret_obj = nullptr;
		class_instance->InvokeMonoMethodOnParentClass(
			*App->scripting->scripting_cluster->beengine_object_class, "SetPointerRef", args, 1, ret_obj);
	}
}

void ScriptingBridgeGameObject::CleanUp()
{

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

GameObject* ScriptingBridgeGameObject::GetGameObjectFromMonoObject(MonoObject * mono_object)
{
	GameObject* ret = nullptr;

	if (mono_object != nullptr)
	{		
		MonoObject* obj_ret = nullptr;
		if (App->scripting->InvokeMonoMethod(mono_object, 
			App->scripting->scripting_cluster->beengine_object_class->GetMonoClass(), "GetPointerRef", nullptr, 0, obj_ret))
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

MonoObject* ScriptingBridgeGameObject::AddComponent(MonoObject * mono_object, MonoString * component_type)
{
	MonoObject* ret = nullptr;

	GameObject* go = GetGameObjectFromMonoObject(mono_object);

	if (go != nullptr)
	{
		std::string type_name = App->scripting->UnboxString(component_type);

		ComponentType type = App->gameobject->GetComponentTypeByComponentScriptingName(type_name);

		//// Button
		//if (type.compare(App->scripting->scripting_cluster->component_button_class.GetName()) == 0)
		//{
		//	ComponentButton* button = (ComponentButton*)go->CreateComponent(ComponentType::COMPONENT_TYPE_BUTTON);

		//	ret = button->GetScriptingBridge()->GetComponentButtonScriptingInstance()->GetMonoObject();
		//}
	}

	if (ret == nullptr)
		CONSOLE_ERROR("Component %s could not be created, as it does not exist on Bridge Code");


	return ret;
}
