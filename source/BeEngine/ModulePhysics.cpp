#include "ModulePhysics.h"
#include "PhysicsBody.h"
#include "PhysicsShape.h"
#include "PhysicsShapePolygon.h"
#include "ModuleEvent.h"
#include "ModuleState.h"

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

	App->event->Suscribe(std::bind(&ModulePhysics::OnEvent, this, std::placeholders::_1), EventType::EDITOR_GOES_TO_PLAY);
	App->event->Suscribe(std::bind(&ModulePhysics::OnEvent, this, std::placeholders::_1), EventType::EDITOR_GOES_TO_IDLE);

	b2world = new b2World(b2Vec2(0, 0));

	b2world = new b2World(b2Vec2(0, 0));
	b2world->SetContactListener(this);

	SetWorldGravity(float2(0, -9.2f));

	return ret;
}

bool ModulePhysics::CleanUp()
{
	bool ret = true;

	INTERNAL_LOG("Destroying Physics World");

	RELEASE(b2world);

	App->event->UnSuscribe(std::bind(&ModulePhysics::OnEvent, this, std::placeholders::_1), EventType::EDITOR_GOES_TO_PLAY);
	App->event->UnSuscribe(std::bind(&ModulePhysics::OnEvent, this, std::placeholders::_1), EventType::EDITOR_GOES_TO_IDLE);

	return ret;
}

bool ModulePhysics::PreUpdate()
{
	bool ret = true;

	if (App->state->GetEditorUpdateState() == EditorUpdateState::EDITOR_UPDATE_STATE_PLAY)
	{
		b2world->Step(App->GetDT(), 128, 128);

		for (b2Contact* c = b2world->GetContactList(); c; c = c->GetNext())
		{
			if (c->IsTouching())
			{
				PhysicsBody* pb1 = (PhysicsBody*)c->GetFixtureA()->GetBody()->GetUserData();
				PhysicsBody* pb2 = (PhysicsBody*)c->GetFixtureB()->GetBody()->GetUserData();
			}
		}
	}

	return ret;
}

void ModulePhysics::OnEvent(Event * ev)
{
	switch (ev->GetType())
	{
	case EventType::EDITOR_GOES_TO_PLAY:
	{

		break;
	}

	case EventType::EDITOR_GOES_TO_IDLE:
	{

		break;
	}
	}
}

void ModulePhysics::BeginContact(b2Contact * contact)
{
	if (App->state->GetEditorUpdateState() == EditorUpdateState::EDITOR_UPDATE_STATE_PLAY)
	{
		PhysicsBody* physA = (PhysicsBody*)contact->GetFixtureA()->GetBody()->GetUserData();
		PhysicsBody* physB = (PhysicsBody*)contact->GetFixtureB()->GetBody()->GetUserData();
	}
}

void ModulePhysics::EndContact(b2Contact * contact)
{
	if (App->state->GetEditorUpdateState() == EditorUpdateState::EDITOR_UPDATE_STATE_PLAY)
	{

	}
}

PhysicsBody * ModulePhysics::CreatePhysicsBody()
{
	PhysicsBody* body = new PhysicsBody();

	b2BodyDef b2body_def;
	b2body_def.type = b2BodyType::b2_dynamicBody;
	b2body_def.position.Set(PIXELS_TO_METERS(0), PIXELS_TO_METERS(0));
	b2body_def.gravityScale = 0;
	b2body_def.allowSleep = false;

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
		vertices.push_back(float2(-1, 2));
		vertices.push_back(float2(-1, 0));
		vertices.push_back(float2(0, -3));
		vertices.push_back(float2(1, 0));
		vertices.push_back(float2(1, 1));

		//rtices[0].Set(-1, 2);
		//vertices[1].Set(-1, 0);
		//vertices[2].Set(0, -3);
		//vertices[3].Set(1, 0);
		//vertices[4].Set(1, 1);

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

void ModulePhysics::SetWorldGravity(const float2 & gravity)
{
	b2world->SetGravity(b2Vec2(gravity.x, gravity.y));
}

float2 ModulePhysics::GetWorldGravity() const
{
	b2Vec2 b2_gravity = b2world->GetGravity();

	return float2(b2_gravity.x, b2_gravity.y);
}

void ModulePhysics::RenderGuizmos()
{
	if (render_all_guizmos)
	{
		for (std::vector<PhysicsBody*>::iterator it = bodies.begin(); it != bodies.end(); ++it)
		{
			std::vector<PhysicsShape*> shapes = (*it)->GetShapes();

			for (std::vector<PhysicsShape*>::iterator sh = shapes.begin(); sh != shapes.end(); ++sh)
			{
				(*sh)->RenderGuizmo();
			}
		}
	}
}

void ModulePhysics::SetRenderAllGuizmos(bool set)
{
	render_all_guizmos = set;
}

bool ModulePhysics::GetRenderAllGuizmos() const
{
	return render_all_guizmos;
}
