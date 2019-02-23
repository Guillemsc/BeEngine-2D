#include "PhysicsBody.h"

PhysicsBody::PhysicsBody(PhysicsBodyType body_shape)
{
	physics_body_shape = body_shape;
}

PhysicsBodyType PhysicsBody::GetBodyShape() const
{
	return physics_body_shape;
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

void PhysicsBody::AddChildBody(PhysicsBody * body)
{
	if (body != nullptr && body != this && body->parent_body == nullptr)
	{
		bool exists = false;
		for (std::vector<PhysicsBody*>::iterator it = child_bodies.begin(); it != child_bodies.end(); ++it)
		{
			if ((*it) == body)
			{
				exists = true;
				break;
			}
		}

		if (!exists)
		{
			body->parent_body = this;
			child_bodies.push_back(body);
		}
	}
}

void PhysicsBody::RemoveChildBody(PhysicsBody * body)
{
	if (body != nullptr && body->parent_body != nullptr)
	{
		for (std::vector<PhysicsBody*>::iterator it = child_bodies.begin(); it != child_bodies.end(); ++it)
		{
			if ((*it) == body)
			{
				body->parent_body = nullptr;
				child_bodies.erase(it);
				break;
			}
		}
	}
}

void PhysicsBody::DestroyAllFixtures()
{
	b2Fixture* fixture = b2body->GetFixtureList();

	while (fixture != NULL)
	{
		b2Fixture* to_destroy = fixture;

		fixture = fixture->GetNext();

		b2body->DestroyFixture(to_destroy);
	}
}