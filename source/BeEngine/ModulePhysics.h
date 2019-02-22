#ifndef __MODULE_PHYSICS_H__
#define __MODULE_PHYSICS_H__

#include "Module.h"

#include "Box2D/Box2D/Box2D.h"

class PhysicsBody
{
	PhysicsBody();
};

class ModulePhysics : public Module, public b2ContactListener
{
public:
	ModulePhysics();
	~ModulePhysics();

	bool Awake();
	bool CleanUp();

	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);

private:
	b2World* b2_world = nullptr;
};

#endif // !__MODULE_PHYSICS_H__