#include "ScriptingBridgeScene.h"
#include "App.h"
#include "ModuleGameObject.h"
#include "ScriptingBridgeGameObject.h"
#include "ModuleScripting.h"
#include "ScriptingCluster.h"
#include "ScriptingBridgeBeObject.h"

ScriptingBridgeScene::ScriptingBridgeScene()
	: ScriptingBridgeObject(nullptr)
{
}

ScriptingBridgeScene::~ScriptingBridgeScene()
{
}

void ScriptingBridgeScene::Start()
{
}

void ScriptingBridgeScene::RebuildInstances()
{
}

void ScriptingBridgeScene::PostRebuildInstances()
{
}

void ScriptingBridgeScene::CleanUp()
{
}

MonoObject * ScriptingBridgeScene::CreateGameObject()
{
	GameObject* go = App->gameobject->CreateGameObject();

	return go->GetScriptingBridge()->GetInstance()->GetMonoObject();
}

void ScriptingBridgeScene::DestroyGameObject(MonoObject * mono_object)
{
	GameObject* go = (GameObject*)ScriptingBridgeBeObject::GetBeObjectRefPointer(mono_object);

	if (go != nullptr)
	{
		MonoObject* obj_ret = nullptr;
		if (App->scripting->InvokeMonoMethod(mono_object,
			App->scripting->scripting_cluster->beengine_object_class->GetMonoClass(), "ClearPointerRef", nullptr, 0, obj_ret));

		App->gameobject->DestroyGameObject(go);
	}
}

void ScriptingBridgeScene::DestroyComponent(MonoObject * mono_object)
{
	if (mono_object != nullptr)
	{
		GameObjectComponent* comp = nullptr;

		MonoObject* obj_ret = nullptr;
		if (App->scripting->InvokeMonoMethod(mono_object,
			App->scripting->scripting_cluster->beengine_object_class->GetMonoClass(), "GetPointerRef", nullptr, 0, obj_ret))
		{
			MonoObject* obj_ret = nullptr;
			if (App->scripting->InvokeMonoMethod(mono_object,
				App->scripting->scripting_cluster->beengine_object_class->GetMonoClass(), "ClearPointerRef", nullptr, 0, obj_ret));

			if (obj_ret != nullptr)
			{
				comp = (GameObjectComponent*)App->scripting->UnboxPointer((MonoString*)obj_ret);

				if (comp != nullptr)
				{
					comp->GetOwner()->DestroyComponent(comp);
				}
			}
		}
	}
}

void ScriptingBridgeScene::LoadScene(MonoString * mono_string)
{
}
