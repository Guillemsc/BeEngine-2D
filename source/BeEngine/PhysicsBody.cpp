#include "PhysicsBody.h"
#include "PhysicsShape.h"

PhysicsBody::PhysicsBody()
{
	
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

	shapes.clear();
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

void PhysicsBody::SetType(PhysicsBodyType type)
{
	b2BodyType b2type;

	switch (type)
	{
	case PHYSICS_BODY_KINEMATIC:
		b2type = b2BodyType::b2_kinematicBody;
		break;
	case PHYSICS_BODY_DYNAMIC:
		b2type = b2BodyType::b2_dynamicBody;
		break;
	case PHYSICS_BODY_STATIC:
		b2type = b2BodyType::b2_staticBody;
		break;
	default:
		break;
	}

	b2body->SetType(b2type);
}

void PhysicsBody::SetPosition(const float2 & pos)
{
	b2body->SetTransform(b2Vec2(pos.x, pos.y), b2body->GetAngle());
}

float2 PhysicsBody::GetPosition() const
{
	float2 ret = float2::zero;

	b2Vec2 pos = b2body->GetPosition();

	ret.x = METERS_TO_PIXELS(pos.x);
	ret.y = METERS_TO_PIXELS(pos.y);

	return ret;
}

void PhysicsBody::SetRotationDegrees(float angle)
{
	b2body->SetTransform(b2body->GetPosition(), angle * DEGTORAD);
}

float PhysicsBody::GetRotationDegrees() const
{
	return b2body->GetAngle() * RADTODEG;
}

void PhysicsBody::SetFixedRotationDegrees(float angle)
{
	b2body->SetFixedRotation(angle * DEGTORAD);
}

void PhysicsBody::AddForce(const float2 & force, const float2 & point)
{
	b2body->ApplyForce(b2Vec2(force.x, force.y), b2Vec2(point.x, point.y), true);
}

void PhysicsBody::AddForceToCenter(const float2 & force)
{
	b2body->ApplyForceToCenter(b2Vec2(force.x, force.y), true);
}

void PhysicsBody::AddImpulse(const float2 & force, const float2 & point)
{
	b2body->ApplyLinearImpulse(b2Vec2(force.x, force.y), b2Vec2(point.x, point.y), true);
}

void PhysicsBody::AddTorque(float torque)
{
	b2body->ApplyTorque(torque, true);
}

void PhysicsBody::AddAngularImpulse(float impulse)
{
	b2body->ApplyAngularImpulse(impulse, true);
}

void PhysicsBody::SetVelocity(const float2 & vel)
{
	b2body->SetLinearVelocity(b2Vec2(vel.x, vel.y));
}

void PhysicsBody::SetAnguladDamping(float set)
{
	b2body->SetAngularDamping(set);
}

void PhysicsBody::SetGravityScale(float scale)
{
	b2body->SetGravityScale(scale);
}

void PhysicsBody::SetBullet(bool set)
{
	b2body->SetBullet(set);
}

void PhysicsBody::SetCanSleep(bool set)
{
	b2body->SetSleepingAllowed(set);
}
