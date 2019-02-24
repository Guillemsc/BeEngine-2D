#ifndef __PHYSICS_BODY_H__
#define __PHYSICS_BODY_H__

#include "ModulePhysics.h"

class PhysicsShape;

class PhysicsBody
{
	friend class ModulePhysics;

private:
	void operator delete(void *) {}

public:
	PhysicsBody();

	float2 GetPosition() const;
	float GetRotationDegrees() const;

	bool Contains(const float2& point);

	void AddShape(PhysicsShape* shape);
	void RemoveShape(PhysicsShape* shape);
	void RemoveAllShapes();

	void CreateFixtures();
	void DestroyFixtures();

private:
	b2Body* b2body = nullptr;

	std::vector<PhysicsShape*> shapes;
};

#endif // !__PHYSICS_BODY_H__