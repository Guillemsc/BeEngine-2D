#ifndef __PHYSICS_BODY_POLYGON_H__
#define __PHYSICS_BODY_POLYGON_H__

#include "ModulePhysics.h"
#include "PhysicsBody.h"

class PhysicsBodyPolygon : public PhysicsBody
{
	friend class ModulePhysics;

private:
	void operator delete(void *) {}

public:
	PhysicsBodyPolygon();

	void CreateFixture();
	void DestroyFixture();

	void SetVertices(const std::vector<float2> set);
	std::vector<float2> GetVertices() const;

private:
	b2PolygonShape polygon_shape;

	std::vector<float2> vertices;

};

#endif // !__PHYSICS_BODY_POLYGON_H__