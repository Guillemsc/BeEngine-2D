#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include <vector>

#include "Globals.h"

class GameObject 
{
	friend class ModuleGameObject;

private:
	void operator delete(void *) {}

public:
	GameObject() {};
	~GameObject() {};

	void Start();
	void CleanUp();

	void SetName(const char* set);

	void SetParent(GameObject* set); 
	void RemoveParent();
	uint GetChildsCount() const;
	GameObject* GetChild(uint index) const;
	bool IsChild(GameObject* go);
	bool IsInChildTree(GameObject* go);

private:
	GameObject* parent = nullptr;
	std::vector<GameObject*> childs;

	std::string name;
};

#endif // !__GAME_OBJECT_H__