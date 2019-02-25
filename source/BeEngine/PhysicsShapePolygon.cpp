#include "PhysicsShapePolygon.h"

PhysicsShapePolygon::PhysicsShapePolygon() : PhysicsShape(PhysicsShapeType::PHYSICS_SHAPE_POLYGON)
{
}

void PhysicsShapePolygon::CreateFixture()
{
}

void PhysicsShapePolygon::DestroyFixture()
{
}

void PhysicsShapePolygon::SetVertices(const std::vector<float2> set)
{
	b2Vec2* p = new b2Vec2[set.size()];

	vertices = set;

	int counter = 0;
	for (std::vector<float2>::const_iterator it = set.begin(); it != set.end(); ++it, ++counter)
	{
		float2 point = (*it);

		p[counter] = b2Vec2(PIXEL_TO_METERS(point.x), PIXEL_TO_METERS(point.y));
	}

	b2polygon_shape.Set(p, set.size());
}

std::vector<float2> PhysicsShapePolygon::GetVertices() const
{
	return vertices;
}
