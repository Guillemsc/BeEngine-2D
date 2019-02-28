#ifndef __MODULE_PHYSICS_H__
#define __MODULE_PHYSICS_H__

#include "Module.h"
#include "Box2D.h"
#include "GeometryMath.h"

#define PIXELS_PER_METER 100.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.01f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) round(PIXELS_PER_METER * m))
#define PIXELS_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

class Event;
class PhysicsBody;
class PhysicsShape;
enum PhysicsShapeType;

class ModulePhysics : public Module, public b2ContactListener
{
public:
	ModulePhysics();
	~ModulePhysics();

	bool Awake();
	bool CleanUp();
	bool PreUpdate();
	void OnEvent(Event* ev);

	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);

	PhysicsBody* CreatePhysicsBody();
	void DestroyPhysicsBody(PhysicsBody* body);

	PhysicsShape* CreatePhysicsShape(PhysicsShapeType type);
	void DestroyPhysicsShape(PhysicsShape* shape);

	void SetWorldGravity(const float2& gravity);
	float2 GetWorldGravity() const;

	void RenderGuizmos();
	void SetRenderAllGuizmos(bool set);
	bool GetRenderAllGuizmos() const;

private:
	std::vector<PhysicsBody*> bodies;
	std::vector<PhysicsShape*> shapes;

private:
	b2World* b2world = nullptr;

	bool render_all_guizmos = false;
};

#endif // !__MODULE_PHYSICS_H__