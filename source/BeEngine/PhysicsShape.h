#ifndef __PHYSICS_SHAPE_H__
#define __PHYSICS_SHAPE_H__

#include "ModulePhysics.h"

enum PhysicsShapeType
{
	PHYSICS_SHAPE_POLYGON,
};

class PhysicsShape
{
	friend class ModulePhysics;
	friend class PhysicsBody;

private:
	void operator delete(void *) {}

public:
	PhysicsShape(PhysicsShapeType type);

	PhysicsShapeType GetBodyShape() const;

	virtual void CreateFixture() {};
	virtual void DestroyFixture() {};

private:
	PhysicsShapeType shape_type;

	PhysicsBody* attached_body = nullptr;
};

#endif // !__PHYSICS_BODY_H__