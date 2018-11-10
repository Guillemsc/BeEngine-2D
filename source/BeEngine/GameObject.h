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
	GameObject(std::string uid);
	~GameObject() {};

	void Start();
	void CleanUp();

	void SetName(const char* set);
	const char* GetName();

	std::string GetUID();

	void SetParent(GameObject* set); 
	void RemoveParent();
	uint GetChildsCount() const;
	GameObject* GetChild(uint index) const;
	bool IsChild(GameObject* go);
	bool IsInChildTree(GameObject* go);
	std::vector<GameObject*> GetChilds() const;

	GameObject* GetParent() const; 

	bool GetSelected() const;

private:
	GameObject* parent = nullptr;
	std::vector<GameObject*> childs;

	std::string name;
	std::string uid;

	bool selected = false;
	uint tree_node_order = 0;
};

#endif // !__GAME_OBJECT_H__