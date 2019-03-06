#ifndef __PHYSICS_SHAPE_POLYGON_H__
#define __PHYSICS_SHAPE_POLYGON_H__

#include "ModulePhysics.h"
#include "PhysicsShape.h"

class PhysicsShapePolygon : public PhysicsShape
{
	friend class ModulePhysics;

private:
	void operator delete(void *) {}

public:
	PhysicsShapePolygon();

	void CreateFixture();
	void DestroyFixture();

	void SetVertices(const std::vector<float2>& set);
	std::vector<float2> GetVertices() const;

	void SetAmoutVertices(int set);

	bool GetShapeTooSmall() const;
	bool GetNotEnoughVertices() const;

private:
	std::vector<float2> vertices;
	std::vector<b2PolygonShape> b2polygon_shapes;

	bool too_small = false;
	bool not_enough_vertices = false;
};

#endif // !__PHYSICS_BODY_POLYGON_H__