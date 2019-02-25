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

	void SetVertices(const std::vector<float2> set);
	std::vector<float2> GetVertices() const;

private:
	b2Fixture* fixture = nullptr;

	b2PolygonShape b2polygon_shape;
	std::vector<float2> vertices;

};

#endif // !__PHYSICS_BODY_POLYGON_H__