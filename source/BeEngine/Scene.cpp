#include "Scene.h"

Scene::Scene(const std::string& set_uid)
{
	uid = set_uid;
}

void Scene::Start()
{
}

void Scene::CleanUp()
{
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

std::vector<GameObject*> Scene::GetRootGameObjects() const
{
	return root_game_objects;
}
