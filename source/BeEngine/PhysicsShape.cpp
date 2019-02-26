#include "PhysicsShape.h"
#include "PhysicsBody.h"

PhysicsShape::PhysicsShape(PhysicsShapeType type)
{
	shape_type = type;
}

PhysicsShapeType PhysicsShape::GetBodyShape() const
{
	return shape_type;
}

void PhysicsShape::DestroyFixture()
{
	if (attached_body != nullptr)
	{
		for (std::vector<b2Fixture*>::iterator it = fixtures.begin(); it != fixtures.end(); ++it)
		{
			attached_body->b2body->DestroyFixture((*it));
		}

		fixtures.clear();
	}
}
