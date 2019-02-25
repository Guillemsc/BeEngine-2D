#include "ModulePhysics.h"
#include "PhysicsBody.h"
#include "PhysicsShape.h"
#include "PhysicsShapePolygon.h"

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

PhysicsShape * ModulePhysics::CreatePhysicsShape(PhysicsShapeType type)
{
	PhysicsShape* ret = nullptr;

	switch (type)
	{
	case PHYSICS_SHAPE_POLYGON:
	{
		ret = new PhysicsShapePolygon();

		std::vector<float2> vertices;
		vertices.push_back(float2(-0.5f, 0.5f));
		vertices.push_back(float2(-0.5f, -0.5f));
		vertices.push_back(float2(0.5f, -0.5f));
		vertices.push_back(float2(0.5f, 0.5f));

		((PhysicsShapePolygon*)ret)->SetVertices(vertices);

		break;
	}

	if (ret != nullptr)
		shapes.push_back(ret);

	}

	return ret;
}

void ModulePhysics::DestroyPhysicsShape(PhysicsShape * shape)
{
	if (shape != nullptr)
	{
		for (std::vector<PhysicsShape*>::iterator it = shapes.begin(); it != shapes.end(); ++it)
		{
			PhysicsShape* curr_shape = (*it);

			if (curr_shape == shape)
			{
				if (curr_shape->attached_body != nullptr)
				{
					curr_shape->attached_body->RemoveShape(curr_shape);
				}

				RELEASE(curr_shape);
				shapes.erase(it);
				break;
			}
		}
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
