#ifndef __PHYSICS_POLYGON_GUIZMO_H__
#define __PHYSICS_POLYGON_GUIZMO_H__

#include "GeometryMath.h"
#include "Guizmo.h"

class ComponentPolygonCollider;

class PhysicsPolygonGuizmo : public Guizmo
{
public:
	PhysicsPolygonGuizmo() {};
	~PhysicsPolygonGuizmo() {};

	void Start();
	void Render(float relative_size);

	void StartEditing(ComponentPolygonCollider* polygon_editing);
	std::vector<float2> FinishEditing(ComponentPolygonCollider* polygon_editing);
	void StopEditing(ComponentPolygonCollider* polygon_editing);
	ComponentPolygonCollider* GetEditingComponent() const;

private: 
	void RebuildHandlers();

private:
	ComponentPolygonCollider* polygon_editing = nullptr;
	std::vector<float2> polygon_points;
	
};

#endif // !__POSITION_GUIZMO_H__