#ifndef __SCENE_H__
#define __SCENE_H__

#include <vector>

#include "Globals.h"

class Scene
{
	friend class ModuleGameObject;
	friend class ScriptingItemGameObject;

private:
	void operator delete(void *) {}

public:
	Scene(std::string uid);
	~Scene() {};

	void Start();
	void CleanUp();

	void SetName(const char* set);
	std::string GetName();

	std::string GetUID();


private:
	std::string name;
	std::string uid;
};

#endif // !__SCENE_H__