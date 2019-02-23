#ifndef __PHYSICS_BODY_H__
#define __PHYSICS_BODY_H__

#include "ModulePhysics.h"

enum PhysicsBodyType
{
	PHYSICS_BODY_CIRCLE,
	PHYSICS_BODY_POLYGON,
};

class PhysicsBody
{
	friend class ModulePhysics;

private:
	void operator delete(void *) {}

public:
	PhysicsBody(PhysicsBodyType body_shape);

	PhysicsBodyType GetBodyShape() const;

	float2 GetPosition() const;
	float GetRotationDegrees() const;

	bool Contains(const float2& point);

	void AddChildBody(PhysicsBody* body);
	void RemoveChildBody(PhysicsBody* body);

private:
	void DestroyAllFixtures();

private:
	b2Body*	b2body = nullptr;

	PhysicsBodyType physics_body_shape;

	PhysicsBody* parent_body = nullptr;
	std::vector<PhysicsBody*> child_bodies;
};

#endif // !__PHYSICS_BODY_H__