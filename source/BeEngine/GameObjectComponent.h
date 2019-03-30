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
	COMPONENT_TYPE_PHYSICS_BODY,
	COMPONENT_TYPE_POLYGON_COLLIDER,
	COMPONENT_TYPE_CANVAS,
	COMPONENT_TYPE_BUTTON,

	COMPONENT_TYPE_UNDEFINED,
};

enum ComponentGroup
{
	TRANSFORMATIONS,
	RENDERING,
	SCRIPTING,
	CAMERA,
	PHYSICS,
	UI,
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
	virtual void OnSaveAbstraction(DataAbstraction& abs) {};
	virtual void OnLoadAbstraction(DataAbstraction& abs) {};

	virtual void Start() {};
	virtual void Update() {};
	virtual void CleanUp() {};

	virtual void OnDestroy() {};
	virtual void OnChildAdded(GameObject* child) {};
	virtual void OnChildRemoved(GameObject* child) {};
	virtual void OnParentChanged(GameObject* new_parent) {};
	virtual void OnChangeActive(bool active) {}

	virtual void OnAddComponent(GameObjectComponent* new_component) {};
	virtual void OnRemoveComponent(GameObjectComponent* new_component) {};

	virtual void OnOwnerSelected() {};
	virtual void OnOwnerDeSelected() {};

	virtual void RenderGuizmos(float relative_size) {};

	GameObject* GetOwner() const;

	ComponentType GetType() const;
	ComponentGroup GetGroup() const;
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