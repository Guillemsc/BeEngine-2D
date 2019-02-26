#include "PhysicsShapePolygon.h"
#include "PhysicsBody.h"

PhysicsShapePolygon::PhysicsShapePolygon() : PhysicsShape(PhysicsShapeType::PHYSICS_SHAPE_POLYGON)
{
}

void PhysicsShapePolygon::CreateFixture()
{
	if (attached_body != nullptr && fixtures.size() == 0)
	{
		b2FixtureDef b2fixture;
		b2fixture.density = 0;
		b2fixture.shape = &b2polygon_shape;

		b2Fixture* polygon = attached_body->b2body->CreateFixture(&b2fixture);
		fixtures.push_back(polygon);
	}
}

void PhysicsShapePolygon::SetVertices(const std::vector<float2> set)
{
	b2Vec2* p = new b2Vec2[set.size()];

	vertices = set;

	int counter = 0;
	for (std::vector<float2>::const_iterator it = set.begin(); it != set.end(); ++it, ++counter)
	{
		float2 point = (*it);

		p[counter] = b2Vec2(PIXELS_TO_METERS(point.x), PIXELS_TO_METERS(point.y));
	}

	b2polygon_shape.Set(p, set.size());
}

std::vector<float2> PhysicsShapePolygon::GetVertices() const
{
	return vertices;
}
