#include "PhysicsBody.h"
#include "PhysicsShape.h"

PhysicsBody::PhysicsBody()
{
	
}

float2 PhysicsBody::GetPosition() const
{
	float2 ret = float2::zero;

	b2Vec2 pos = b2body->GetPosition();

	ret.x = METERS_TO_PIXELS(pos.x);
	ret.y = METERS_TO_PIXELS(pos.y);

	return ret;
}

float PhysicsBody::GetRotationDegrees() const
{
	return b2body->GetAngle() * RADTODEG;
}

bool PhysicsBody::Contains(const float2 & point)
{
	bool ret = false;

	b2Vec2 b2point(PIXEL_TO_METERS(point.x), PIXEL_TO_METERS(point.y));

	const b2Fixture* fixture = b2body->GetFixtureList();

	while (fixture != NULL)
	{
		if (fixture->GetShape()->TestPoint(b2body->GetTransform(), b2point))
		{
			ret = true;
			break;
		}

		if (!ret)
			fixture = fixture->GetNext();
	}

	return ret;
}

void PhysicsBody::AddShape(PhysicsShape * shape)
{
	if (shape != nullptr && shape->attached_body == nullptr)
	{
		bool exists = false;
		for (std::vector<PhysicsShape*>::iterator it = shapes.begin(); it != shapes.end(); ++it)
		{
			if ((*it) == shape)
			{
				exists = true;
				break;
			}
		}

		if (!exists)
		{
			shapes.push_back(shape);
			shape->attached_body = this;
		}
	}
}

void PhysicsBody::RemoveShape(PhysicsShape * shape)
{
	if (shape != nullptr && shape->attached_body != nullptr)
	{
		for (std::vector<PhysicsShape*>::iterator it = shapes.begin(); it != shapes.end(); ++it)
		{
			if ((*it) == shape)
			{
				shape->DestroyFixture();
				shape->attached_body = nullptr;

				shapes.erase(it);
				break;
			}
		}
	}
}

void PhysicsBody::RemoveAllShapes()
{
	for (std::vector<PhysicsShape*>::iterator it = shapes.begin(); it != shapes.end();)
	{		
		(*it)->DestroyFixture();
		(*it)->attached_body = nullptr;
	}

	shapes.clear()
}

void PhysicsBody::CreateFixtures()
{
	for (std::vector<PhysicsShape*>::iterator it = shapes.begin(); it != shapes.end(); ++it)
	{
		(*it)->CreateFixture();
	}
}

void PhysicsBody::DestroyFixtures()
{
	for (std::vector<PhysicsShape*>::iterator it = shapes.begin(); it != shapes.end(); ++it)
	{
		(*it)->DestroyFixture();
	}
}