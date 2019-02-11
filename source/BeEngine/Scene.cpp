#include "Scene.h"

Scene::Scene()
{
	
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

bool Scene::GetSelected() const
{
	return selected;
}

std::vector<GameObject*> Scene::GetRootGameObjects() const
{
	return root_game_objects;
}
