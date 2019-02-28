#include "PhysicsShape.h"
#include "PhysicsBody.h"

PhysicsShape::PhysicsShape(PhysicsShapeType type)
{
	shape_type = type;

	SetIsSensor(false);
}

PhysicsShapeType PhysicsShape::GetBodyShape() const
{
	return shape_type;
}

void PhysicsShape::SetIsSensor(bool set)
{
	is_sensor = set;

	for (std::vector<b2Fixture*>::iterator it = fixtures.begin(); it != fixtures.end(); ++it)
	{
		(*it)->SetSensor(is_sensor);
	}
}

bool PhysicsShape::GetIsSensor() const
{
	return is_sensor;
}
