#ifndef __PHYSICS_POLYGON_GUIZMO_H__
#define __PHYSICS_POLYGON_GUIZMO_H__

#include "GeometryMath.h"
#include "Guizmo.h"

class ComponentPolygonCollider;
class GameObject;

class PhysicsPolygonGuizmo : public Guizmo
{
public:
	PhysicsPolygonGuizmo() {};
	~PhysicsPolygonGuizmo() {};

	void Start();
	void Render(float relative_size, const float2& mouse_pos);

	void StartEditing(ComponentPolygonCollider* polygon_editing);
	std::vector<float2> FinishEditing(ComponentPolygonCollider* polygon_editing);
	void StopEditing(ComponentPolygonCollider* polygon_editing);
	ComponentPolygonCollider* GetEditingComponent() const;

	GameObject* GetEditingGameObject() const;

private: 
	void RebuildHandlers();

	void AddPoint(int index, const float2& point);

private:
	ComponentPolygonCollider* polygon_editing = nullptr;
	std::vector<float2> polygon_points;	

	GuizmoHandler* add_point_handler = nullptr;
	bool add_point = false;
};

#endif // !__POSITION_GUIZMO_H__