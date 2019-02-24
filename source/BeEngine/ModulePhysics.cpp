#include "ModulePhysics.h"
#include "PhysicsBody.h"
#include "PhysicsBodyPolygon.h"

#ifdef _DEBUG
#pragma comment( lib, "Box2D/libx86/Debug/Box2D.lib" )
#else
#pragma comment( lib, "Box2D/libx86/Release/Box2D.lib" )
#endif

ModulePhysics::ModulePhysics()
{
}

ModulePhysics::~ModulePhysics()
{
}

bool ModulePhysics::Awake()
{
	bool ret = true;

	INTERNAL_LOG("Creating Physics World");

	b2world = new b2World(b2Vec2(0, 0));

	b2world = new b2World(b2Vec2(0, 0));
	b2world->SetContactListener(this);

	return ret;
}

bool ModulePhysics::CleanUp()
{
	bool ret = true;

	INTERNAL_LOG("Destroying Physics World");

	RELEASE(b2world);

	return ret;
}

void ModulePhysics::BeginContact(b2Contact * contact)
{
}

void ModulePhysics::EndContact(b2Contact * contact)
{
}

PhysicsBody * ModulePhysics::CreatePhysicsBody()
{
	PhysicsBody* body = new PhysicsBody();

	b2BodyDef b2body_def;
	b2body_def.type = b2BodyType::b2_staticBody;
	b2body_def.position.Set(PIXEL_TO_METERS(0), PIXEL_TO_METERS(0));
	b2body_def.gravityScale = 0;

	b2Body* b2body = b2world->CreateBody(&b2body_def);

	body->b2body = b2body;

	b2body->SetUserData(body);

	bodies.push_back(body);

	return body;
}

void ModulePhysics::DestroyPhysicsBody(PhysicsBody * body)
{
	if (body != nullptr)
	{
		body->RemoveAllShapes();

		b2world->DestroyBody(body->b2body);

		RELEASE(body);
	}
}

void ModulePhysics::CreateFixtures()
{
	for (std::vector<PhysicsBody*>::iterator it = bodies.begin(); it != bodies.end(); ++it)
	{
		(*it)->CreateFixtures();
	}
}

void ModulePhysics::DestroyFixtures()
{
	for (std::vector<PhysicsBody*>::iterator it = bodies.begin(); it != bodies.end(); ++it)
	{
		(*it)->DestroyFixtures();
	}
}
