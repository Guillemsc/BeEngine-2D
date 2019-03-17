#include "PhysicsShape.h"
#include "PhysicsBody.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

PhysicsShape::PhysicsShape(PhysicsShapeType type)
{
	shape_type = type;

	SetIsSensor(false);
}

PhysicsShapeType PhysicsShape::GetBodyShapeType() const
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

void PhysicsShape::SetDensity(float set)
{
	density = set;

	if (density < 0.0f)
		density = 0.0f;

	for (std::vector<b2Fixture*>::iterator it = fixtures.begin(); it != fixtures.end(); ++it)
	{
		(*it)->SetDensity(density);
	}
}

float PhysicsShape::GetDensity() const
{
	return density;
}

void PhysicsShape::SetFriction(float set)
{
	friction = set;

	if (friction < 0.0f)
		friction = 0.0f;

	for (std::vector<b2Fixture*>::iterator it = fixtures.begin(); it != fixtures.end(); ++it)
	{
		(*it)->SetFriction(friction);
	}
}

float PhysicsShape::GetFriction() const
{
	return friction;
}

PhysicsBody * PhysicsShape::GetAttachedBody() const
{
	return attached_body;
}

std::vector<std::vector<float2>> PhysicsShape::GetTriangles() const
{
	return triangles;
}
