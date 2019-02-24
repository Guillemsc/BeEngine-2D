#include "PhysicsBodyPolygon.h"

PhysicsBodyPolygon::PhysicsBodyPolygon() : PhysicsBody(PhysicsBodyType::PHYSICS_BODY_POLYGON)
{
}

void PhysicsBodyPolygon::SetVertices(const std::vector<float2> set)
{
	b2Vec2* p = new b2Vec2[set.size()];

	vertices = set;

	int counter = 0;
	for (std::vector<float2>::const_iterator it = set.begin(); it != set.end(); ++it, ++counter)
	{
		p[counter] = b2Vec2(PIXEL_TO_METERS(-0.5f), PIXEL_TO_METERS(0.5f));
	}

	b2Vec2* p = new b2Vec2[4];

	polygon_shape.Set(p, set.size());
}

std::vector<float2> PhysicsBodyPolygon::GetVertices() const
{
	return vertices;
}
