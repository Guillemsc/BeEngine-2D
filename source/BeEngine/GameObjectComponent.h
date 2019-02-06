#ifndef __GAME_OBJECT_COMPONENT_H__
#define __GAME_OBJECT_COMPONENT_H__

#include <vector>

#include "Globals.h"

class GameObject;
class DataAbstraction;

enum ComponentType
{
	COMPONENT_TYPE_TRANSFORM,
	COMPONENT_TYPE_SPRITE_RENDERER,
	COMPONENT_TYPE_SCRIPT,
	COMPONENT_TYPE_CAMERA,
};

enum ComponentGroup
{
	TRANSFORMATIONS,
	RENDERING,
	SCRIPTING,
	CAMERA,
};

class GameObjectComponent
{
	friend class GameObject;

private:
	void operator delete(void *) {}

public:
	GameObjectComponent(std::string name, const ComponentType& type, const ComponentGroup& group, bool unique_per_game_object = false, bool can_destroy = true);
	~GameObjectComponent() {};

	virtual void EditorDraw() {};
	virtual void OnSaveAbstraction(const DataAbstraction& abs) {};
	virtual void OnLoadAbstraction(const DataAbstraction& abs) {};

	virtual void Start() {};
	virtual void Update() {};
	virtual void CleanUp() {};


	virtual void OnChildAdded(GameObject* child) {};
	virtual void OnChildRemoved(GameObject* child) {};
	virtual void OnParentChanged(GameObject* new_parent) {};

	GameObject* GetOwner();

	ComponentType GetType() const;
	std::string GetName() const;
	std::string GetUID() const;

	bool GetCanDestroy() const;

protected:
	GameObject* owner = nullptr;

private:
	std::string name;
	std::string uid;
	ComponentType type;
	ComponentGroup group;
	bool can_destroy = true;
	bool unique_per_game_object = false;
};

#endif // !__GAME_OBJECT_COMPONENT_H__