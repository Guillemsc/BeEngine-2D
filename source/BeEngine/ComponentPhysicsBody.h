#ifndef __COMPONENT_PHYSICS_BODY_H__
#define __COMPONENT_PHYSICS_BODY_H__

#include "GameObjectComponent.h"

#include "GeometryMath.h"

class GameObject;
class Event;
class PhysicsBody;

class ComponentPhysicsBody : public GameObjectComponent
{
	friend class GameObject;

private:
	void operator delete(void *) {}

public:
	ComponentPhysicsBody();
	~ComponentPhysicsBody();

	void EditorDraw();

	void Start();
	void Update();
	void CleanUp();

	void OnSaveAbstraction(DataAbstraction& abs);
	void OnLoadAbstraction(DataAbstraction& abs);

	void OnEvent(Event* ev);

	void OnChildAdded(GameObject* child);
	void OnChildRemoved(GameObject* child);
	void OnParentChanged(GameObject* new_parent);

private:
	PhysicsBody* physics_body = nullptr;

};

#endif // !__COMPONENT_PHYSICS_BODY_H__