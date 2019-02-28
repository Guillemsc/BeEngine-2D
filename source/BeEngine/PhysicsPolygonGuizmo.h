#ifndef __PHYSICS_POLYGON_GUIZMO_H__
#define __PHYSICS_POLYGON_GUIZMO_H__

#include "GeometryMath.h"
#include "Guizmo.h"

class PhysicsShapePolygon;

class PhysicsPolygonGuizmo : public Guizmo
{
public:
	PhysicsPolygonGuizmo() {};
	~PhysicsPolygonGuizmo() {};

	void Start();
	void Render(float relative_size);

private:
	PhysicsShapePolygon* polygon_editing = nullptr;

	bool edited = false;
};

#endif // !__POSITION_GUIZMO_H__