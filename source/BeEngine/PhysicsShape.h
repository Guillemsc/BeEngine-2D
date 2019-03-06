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

	PhysicsShapeType GetBodyShapeType() const;

	virtual void CreateFixture() {};
	virtual void DestroyFixture() {};

	void SetIsSensor(bool set);
	bool GetIsSensor() const;

	PhysicsBody* GetAttachedBody() const;

	std::vector<std::vector<float2>> GetTriangles() const;

protected:
	PhysicsShapeType shape_type;

	std::vector<b2Fixture*> fixtures;

	std::vector<std::vector<float2>> triangles;

	bool is_sensor = false;

	PhysicsBody* attached_body = nullptr;
};

#endif // !__PHYSICS_BODY_H__