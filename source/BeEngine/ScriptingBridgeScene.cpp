#include "ScriptingBridgeScene.h"
#include "App.h"
#include "ModuleGameObject.h"
#include "ScriptingBridgeGameObject.h"
#include "ModuleScripting.h"
#include "ScriptingCluster.h"
#include "ScriptingBridgeBeObject.h"
#include "ResourcePrefab.h"

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
		App->gameobject->DestroyGameObject(go);
	}
}

void ScriptingBridgeScene::DestroyComponent(MonoObject * mono_object)
{	
	GameObjectComponent* comp = (GameObjectComponent*)ScriptingBridgeBeObject::GetBeObjectRefPointer(mono_object);

	if (comp != nullptr)
	{
		comp->GetOwner()->DestroyComponent(comp);
	}
}

MonoObject* ScriptingBridgeScene::LoadPrefab(MonoObject * mono_object)
{
	MonoObject* ret = nullptr;

	ResourcePrefab* res = (ResourcePrefab*)ScriptingBridgeBeObject::GetBeObjectRefPointer(mono_object);

	if (res != nullptr)
	{
		GameObject* go = res->LoadToScene();

		if (go != nullptr)
		{
			ret = go->GetScriptingBridge()->GetInstance()->GetMonoObject();
		}
	}

	return ret;
}

void ScriptingBridgeScene::LoadScene(MonoString * mono_string)
{
}
