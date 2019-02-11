#ifndef __SCENE_H__
#define __SCENE_H__

#include <vector>

#include "Globals.h"
#include "GameObject.h"

class Scene
{
	friend class ModuleGameObject;
	friend class ScriptingItemGameObject;

private:
	void operator delete(void *) {}

public:
	Scene();
	~Scene() {};

	void Start();
	void CleanUp();

	void SetName(const char* set);
	std::string GetName();

	bool GetSelected() const;

	std::vector<GameObject*> GetRootGameObjects() const;

private:
	std::string name;

	bool selected = false;

	std::vector<GameObject*> root_game_objects;
};

#endif // !__SCENE_H__