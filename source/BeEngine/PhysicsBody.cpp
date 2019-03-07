#include "PhysicsBody.h"
#include "PhysicsShape.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

PhysicsBody::PhysicsBody()
{

}

bool PhysicsBody::Contains(const float2 & point)
{
	bool ret = false;

	b2Vec2 b2point(PIXELS_TO_METERS(point.x), PIXELS_TO_METERS(point.y));

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
			shape->attached_body = this;

			shape->CreateFixture();

			shapes.push_back(shape);
		}
	}

	SetCanSleep(false);
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
	for (std::vector<PhysicsShape*>::iterator it = shapes.begin(); it != shapes.end(); ++it)
	{		
		(*it)->DestroyFixture();
		(*it)->attached_body = nullptr;
	}

	shapes.clear();
}

std::vector<PhysicsShape*> PhysicsBody::GetShapes()
{
	return shapes;
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

PhysicsBodyType PhysicsBody::GetType() const
{
	PhysicsBodyType ret = PhysicsBodyType::PHYSICS_BODY_STATIC;

	b2BodyType b2type = b2body->GetType();

	switch (b2type)
	{
	case b2BodyType::b2_kinematicBody:
		ret = PhysicsBodyType::PHYSICS_BODY_KINEMATIC;
		break;
	case b2BodyType::b2_dynamicBody:
		ret = PhysicsBodyType::PHYSICS_BODY_DYNAMIC;
		break;
	case b2BodyType::b2_staticBody:
		ret = PhysicsBodyType::PHYSICS_BODY_STATIC;
		break;	
	}

	return ret;
}

void PhysicsBody::ClearForces()
{
	SetVelocity(float2(0.0f, 0.0f));
	SetAngularVelocity(0.0f);
}

void PhysicsBody::SetPosition(const float2 & pos)
{
	b2body->SetTransform(b2Vec2(PIXELS_TO_METERS(pos.x), PIXELS_TO_METERS(pos.y)), b2body->GetAngle());
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

void PhysicsBody::SetMass(float set)
{
	b2MassData mass;
	mass.mass = set;

	b2body->SetMassData(&mass);
}

float PhysicsBody::GetMass() const
{
	b2MassData mass;
	b2body->GetMassData(&mass);

	return mass.mass;
}

void PhysicsBody::SetFixedRotation(bool set)
{
	b2body->SetFixedRotation(set);
}

bool PhysicsBody::GetFixedRotation() const
{
	return b2body->IsFixedRotation();
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

float2 PhysicsBody::GetVelocity() const
{
	b2Vec2 b2vel = b2body->GetLinearVelocity();

	return float2(b2vel.x, b2vel.y);
}

void PhysicsBody::SetAngularVelocity(float set)
{
	b2body->SetAngularVelocity(set);
}

float PhysicsBody::GetAngularVelocity() const
{
	return b2body->GetAngularVelocity();
}

void PhysicsBody::SetLinearDamping(float set)
{
	b2body->SetLinearDamping(set);
}

float PhysicsBody::GetLinearDamping() const
{
	return b2body->GetLinearDamping();
}

void PhysicsBody::SetAnguladDamping(float set)
{
	b2body->SetAngularDamping(set);
}

float PhysicsBody::GetAngularDamping() const
{
	return b2body->GetAngularDamping();
}

void PhysicsBody::SetGravityScale(float scale)
{
	b2body->SetGravityScale(scale);
}

float PhysicsBody::GetGravityScale() const
{
	return b2body->GetGravityScale();
}

void PhysicsBody::SetBullet(bool set)
{
	b2body->SetBullet(set);
}

void PhysicsBody::SetCanSleep(bool set)
{
	b2body->SetSleepingAllowed(set);
}

float2 PhysicsBody::LocalPointToWorldPoint(const float2& local_point) const
{
	b2Vec2 world_point = b2body->GetWorldPoint(b2Vec2(PIXELS_TO_METERS(local_point.x), PIXELS_TO_METERS(local_point.y)));

	return float2(METERS_TO_PIXELS(world_point.x), METERS_TO_PIXELS(world_point.y));
}

void PhysicsBody::SetComponentTransform(ComponentTransform * set)
{
	component_transform = set;
}

ComponentTransform * PhysicsBody::GetComponentTransform() const
{
	return component_transform;
}