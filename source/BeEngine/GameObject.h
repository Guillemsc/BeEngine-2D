#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#include <vector>

#include "Globals.h"
#include "GameObjectComponent.h"

class DataAbstraction;
class ComponentTransform;
class ScriptingClassInstance;

class GameObject 
{
	friend class ModuleGameObject;
	friend class ScriptingItemGameObject;

private:
	void operator delete(void *) {}

public:
	GameObject(std::string uid);
	~GameObject() {};

	void OnSaveAbstraction(const DataAbstraction& abs);
	void OnLoadAbstraction(const DataAbstraction& abs);

	void Start();
	void Update();
	void CleanUp();

	void SetName(const char* set);
	std::string GetName();

	std::string GetUID();

	void SetParent(GameObject* set); 
	void RemoveParent();
	GameObject* GetParent() const;
	uint GetChildsCount() const;
	GameObject* GetChild(uint index) const;
	bool IsChild(GameObject* go);
	bool IsInChildTree(GameObject* go);
	std::vector<GameObject*> GetChilds() const;

	GameObjectComponent* CreateComponent(const ComponentType& type);
	void DestroyComponent(GameObjectComponent* component, bool check_can_destroy = true);
	bool GetHasComponent(const ComponentType& type) const;
	std::vector<GameObjectComponent*> GetComponents() const;

	bool GetSelected() const;

	ScriptingClassInstance* GetScriptingInstance() const;

private:
	void DestroyAllComponentsNow();
	void ActuallyDestroyComponents();

public:
	ComponentTransform* transform = nullptr;

private:
	GameObject* parent = nullptr;
	std::vector<GameObject*> childs;

	std::vector<GameObjectComponent*> components;
	std::vector<GameObjectComponent*> components_to_destroy;

	std::string name;
	std::string uid;

	bool selected = false;
	uint tree_node_order = 0;

	ScriptingClassInstance* scripting_instance = nullptr;
};

#endif // !__GAME_OBJECT_H__