#include "Scene.h"

Scene::Scene(std::string uid)
{
	this->uid = uid;
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

std::string Scene::GetUID()
{
	return uid;
}
