#include "Scene.h"
#include "App.h"
#include "ModuleEvent.h"
#include "Event.h"
#include "ResourceScene.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

Scene::Scene(const std::string& set_uid)
{
	uid = set_uid;
}

void Scene::Start()
{
	App->event->Suscribe(std::bind(&Scene::OnEvent, this, std::placeholders::_1), EventType::RESOURCE_DESTROYED);
}

void Scene::CleanUp()
{
	App->event->UnSuscribe(std::bind(&Scene::OnEvent, this, std::placeholders::_1), EventType::RESOURCE_DESTROYED);
}

void Scene::OnEvent(Event * ev)
{
	if (ev->GetType() == EventType::RESOURCE_DESTROYED)
	{
		EventResourceDestroyed* c_ev = (EventResourceDestroyed*)ev;

		if (c_ev->GetResource() == resource)
		{
			resource = nullptr;
		}
	}
}

void Scene::SetName(const char * set)
{
	name = set;
}

std::string Scene::GetName()
{
	return name;
}

std::string Scene::GetUid() const
{
	return uid;
}

bool Scene::GetSelected() const
{
	return selected;
}

void Scene::SetResourceScene(ResourceScene * set)
{
	resource = set;
}

ResourceScene * Scene::GetResourceScene() const
{
	return resource;
}

bool Scene::HasResourceScene() const
{
	return resource != nullptr;
}

std::vector<GameObject*> Scene::GetRootGameObjects() const
{
	return root_game_objects;
}
