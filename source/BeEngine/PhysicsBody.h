#ifndef __PHYSICS_BODY_H__
#define __PHYSICS_BODY_H__

#include "ModulePhysics.h"

class PhysicsShape;

enum PhysicsBodyType
{
	PHYSICS_BODY_KINEMATIC,
	PHYSICS_BODY_DYNAMIC,
	PHYSICS_BODY_STATIC,
};

class PhysicsBody
{
	friend class ModulePhysics;

private:
	void operator delete(void *) {}

public:
	PhysicsBody();

	bool Contains(const float2& point);

	void AddShape(PhysicsShape* shape);
	void RemoveShape(PhysicsShape* shape);
	void RemoveAllShapes();

	void CreateFixtures();
	void DestroyFixtures();

	void SetType(PhysicsBodyType type);

	void SetPosition(const float2& pos);
	float2 GetPosition() const;

	void SetRotationDegrees(float angle);
	float GetRotationDegrees() const;
	void SetFixedRotationDegrees(float angle);

	// Applies gradual force on a point of the body
	void AddForce(const float2& force, const float2& point);

	// Applies gradual force on the center point of the body
	void AddForceToCenter(const float2& force);

	// Applies immediate force on on a point of the body
	void AddImpulse(const float2& force, const float2& point);

	// Applies gradual spin force in counter clockwise direction
	void AddTorque(float torque);

	// Applies inmediate spin force in counter clockwise direction
	void AddAngularImpulse(float impulse);

	void SetVelocity(const float2& vel);

	// Angular velocity lost over time
	void SetAnguladDamping(float set);

	void SetGravityScale(float scale);

	void SetBullet(bool set);
	void SetCanSleep(bool set);

private:
	b2Body* b2body = nullptr;

	std::vector<PhysicsShape*> shapes;
};

#endif // !__PHYSICS_BODY_H__