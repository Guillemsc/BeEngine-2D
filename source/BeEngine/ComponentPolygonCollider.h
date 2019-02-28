#ifndef __COMPONENT_POLYGON_COLLIDER_H__
#define __COMPONENT_POLYGON_COLLIDER_H__

#include "GameObjectComponent.h"

#include "GeometryMath.h"

class GameObject;
class Event;
class PhysicsShape;
class PhysicsShapePolygon;

class ComponentPolygonCollider: public GameObjectComponent
{
	friend class GameObject;

private:
	void operator delete(void *) {}

public:
	ComponentPolygonCollider();
	~ComponentPolygonCollider();

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

	void RenderGuizmos();

private:
	PhysicsShapePolygon* physics_shape = nullptr;
};

#endif // !__COMPONENT_POLYGON_COLLIDER_H__