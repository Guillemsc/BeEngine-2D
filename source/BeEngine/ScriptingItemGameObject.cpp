#include "ScriptingItemGameObject.h"
#include "App.h"
#include "GameObject.h"
#include "ModuleGameObject.h"
#include "App.h"
#include "ModuleEvent.h"

ScriptingItemGameObject::ScriptingItemGameObject() : ScriptingItem()
{
}

ScriptingItemGameObject::~ScriptingItemGameObject()
{
}

void ScriptingItemGameObject::Start()
{
	App->event->Suscribe(std::bind(&ScriptingItemGameObject::OnEvent, this, std::placeholders::_1), EventType::GAME_OBJECT_CREATED);
	App->event->Suscribe(std::bind(&ScriptingItemGameObject::OnEvent, this, std::placeholders::_1), EventType::GAME_OBJECT_DESTROYED);
}

void ScriptingItemGameObject::RegisterInternalCalls()
{
	mono_add_internal_call("BeEngine.GameObject::SetName", (const void*)this->SetName);
}

void ScriptingItemGameObject::CleanUp()
{
	App->event->UnSuscribe(std::bind(&ScriptingItemGameObject::OnEvent, this, std::placeholders::_1), EventType::GAME_OBJECT_CREATED);
	App->event->UnSuscribe(std::bind(&ScriptingItemGameObject::OnEvent, this, std::placeholders::_1), EventType::GAME_OBJECT_DESTROYED);

	DestroyInstances();
}

void ScriptingItemGameObject::OnEvent(Event * ev)
{
	switch (ev->GetType())
	{
	case EventType::GAME_OBJECT_CREATED:
	{
		EventGameObjectCreated* c_ev = (EventGameObjectCreated*)ev;

		AddScriptingInstance(c_ev->GetGameObject());

		break;
	}
	case EventType::GAME_OBJECT_DESTROYED:
	{
		EventGameObjectDestroyed* c_ev = (EventGameObjectDestroyed*)ev;

		RemoveScriptingInstance(c_ev->GetGameObject());

		break;
	}
	default:
		break;
	}
}

void ScriptingItemGameObject::RebuildInstances()
{
	std::vector<GameObject*> go = App->gameobject->GetGameObjects();

	for (std::vector<GameObject*>::iterator it = go.begin(); it != go.end(); ++it)
	{
		GameObject* curr_go = (*it);

		RemoveScriptingInstance(curr_go);
		AddScriptingInstance(curr_go);
	}
}

void ScriptingItemGameObject::DestroyInstances()
{
	std::vector<GameObject*> go = App->gameobject->GetGameObjects();

	for (std::vector<GameObject*>::iterator it = go.begin(); it != go.end(); ++it)
	{
		GameObject* curr_go = (*it);

		RemoveScriptingInstance(curr_go);
	}
}

void ScriptingItemGameObject::AddScriptingInstance(GameObject * go)
{
	if (go != nullptr)
	{
		if (go->scripting_instance != nullptr)
			RemoveScriptingInstance(go);

		if (App->scripting->scripting_assembly != nullptr && App->scripting->scripting_assembly->GetAssemblyLoaded())
		{
			ScriptingClass game_object_class;
			if (App->scripting->scripting_assembly->GetClass("BeEngine", "GameObject", game_object_class))
			{
				ScriptingClass be_engine_object_class;
				if (game_object_class.GetParentClass(be_engine_object_class))
				{
					ScriptingClassInstance* ins = game_object_class.CreateInstance();

					if (ins != nullptr)
					{
						MonoArray* mono_pointer = App->scripting->BoxPointer(go);

						void* args[1] = { mono_pointer };

						MonoObject* ret_obj = nullptr;
						if (ins->InvokeMonoMethodOnParentClass(be_engine_object_class, "SetPointerRef", args, 1, ret_obj))
						{
							go->scripting_instance = ins;
						}
						else
						{
							ins->CleanUp();
							RELEASE(ins);
						}
					}
				}
			}
		}
	}
}

void ScriptingItemGameObject::RemoveScriptingInstance(GameObject * go)
{
	if (go != nullptr)
	{
		if (go->scripting_instance != nullptr)
		{
			go->scripting_instance->CleanUp();
			RELEASE(go->scripting_instance);
		}
	}
}

GameObject * ScriptingItemGameObject::GetGameObjectFromMonoObject(MonoObject * mono_object)
{
	GameObject* ret = nullptr;

	if (mono_object != nullptr)
	{
		ScriptingClass game_object_class;
		if (App->scripting->scripting_assembly->GetClass("BeEngine", "GameObject", game_object_class))
		{
			ScriptingClass be_engine_object_class;
			if (game_object_class.GetParentClass(be_engine_object_class))
			{
				MonoObject* obj_ret = nullptr;
				if (App->scripting->InvokeMonoMethod(mono_object, be_engine_object_class.GetMonoClass(), "GetPointerRef", nullptr, 0, obj_ret))
				{
					if (obj_ret != nullptr)
					{
						ret = (GameObject*)App->scripting->UnboxPointer((MonoArray*)obj_ret);
					}
				}
			}
		}
	}

	return ret;
}

void ScriptingItemGameObject::SetName(MonoObject * mono_object, MonoString * mono_string)
{
	GameObject* go = GetGameObjectFromMonoObject(mono_object);

	if (go != nullptr)
	{
		std::string new_name = App->scripting->UnboxString(mono_string);

		go->SetName(new_name.c_str());
	}
}

void ScriptingItemGameObject::GetName()
{
}
