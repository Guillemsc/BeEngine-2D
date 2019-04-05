#include <limits>
#include <array>

#include "ModulePhysics.h"
#include "PhysicsBody.h"
#include "PhysicsShape.h"
#include "PhysicsShapePolygon.h"
#include "ModuleEvent.h"
#include "ModuleState.h"
#include "ComponentTransfrom.h"
#include "GameObject.h"
#include "ScriptingBridgeGameObject.h"
#include "MapboxTriangulation/earcut.hpp"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

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
	b2world->SetContactListener(this);

	SetWorldGravity(float2(0, -9.2f));

	return ret;
}

bool ModulePhysics::CleanUp()
{
	bool ret = true;

	INTERNAL_LOG("Destroying Physics World");

	RELEASE(b2world);

	DestroyAllPhysicsBodies();
	DestroyAllPhysicsShapes();

	App->event->UnSuscribe(std::bind(&ModulePhysics::OnEvent, this, std::placeholders::_1), EventType::EDITOR_GOES_TO_PLAY);
	App->event->UnSuscribe(std::bind(&ModulePhysics::OnEvent, this, std::placeholders::_1), EventType::EDITOR_GOES_TO_IDLE);

	return ret;
}

bool ModulePhysics::PreUpdate()
{
	bool ret = true;

	UpdateContacts();

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
		begin_contacts.push_back(contact);
	}
}

void ModulePhysics::EndContact(b2Contact * contact)
{
	if (App->state->GetEditorUpdateState() == EditorUpdateState::EDITOR_UPDATE_STATE_PLAY)
	{
		end_contacts.push_back(contact);
	}
}

void ModulePhysics::UpdateContacts()
{
	if (App->state->GetEditorUpdateState() == EditorUpdateState::EDITOR_UPDATE_STATE_PLAY)
	{
		b2world->Step(App->GetDT(), 128, 128);

		for (std::vector<b2Contact*>::iterator it = begin_contacts.begin(); it != begin_contacts.end(); ++it)
		{
			b2Contact* contact = (*it);

			PhysicsBody* pb1 = (PhysicsBody*)contact->GetFixtureA()->GetBody()->GetUserData();
			PhysicsBody* pb2 = (PhysicsBody*)contact->GetFixtureB()->GetBody()->GetUserData();

			ComponentTransform* trans_pb1 = pb1->GetComponentTransform();
			ComponentTransform* trans_pb2 = pb2->GetComponentTransform();

			ScriptingBridgeGameObject* bridge_go1 = (ScriptingBridgeGameObject*)trans_pb1->GetOwner()->GetScriptingBridge();
			ScriptingBridgeGameObject* bridge_go2 = (ScriptingBridgeGameObject*)trans_pb2->GetOwner()->GetScriptingBridge();

			bridge_go1->CallOnCollisionEnter(pb2);
			bridge_go2->CallOnCollisionEnter(pb1);
		}

		begin_contacts.clear();

		for (std::vector<b2Contact*>::iterator it = begin_contacts.begin(); it != begin_contacts.end(); ++it)
		{
			b2Contact* contact = (*it);

			PhysicsBody* pb1 = (PhysicsBody*)contact->GetFixtureA()->GetBody()->GetUserData();
			PhysicsBody* pb2 = (PhysicsBody*)contact->GetFixtureB()->GetBody()->GetUserData();

			ComponentTransform* trans_pb1 = pb1->GetComponentTransform();
			ComponentTransform* trans_pb2 = pb2->GetComponentTransform();

			ScriptingBridgeGameObject* bridge_go1 = (ScriptingBridgeGameObject*)trans_pb1->GetOwner()->GetScriptingBridge();
			ScriptingBridgeGameObject* bridge_go2 = (ScriptingBridgeGameObject*)trans_pb2->GetOwner()->GetScriptingBridge();

			bridge_go1->CallOnCollisionExit(pb2);
			bridge_go2->CallOnCollisionExit(pb1);
		}

		begin_contacts.clear();

		for (b2Contact* c = b2world->GetContactList(); c; c = c->GetNext())
		{
			if (c->IsTouching())
			{
				PhysicsBody* pb1 = (PhysicsBody*)c->GetFixtureA()->GetBody()->GetUserData();
				PhysicsBody* pb2 = (PhysicsBody*)c->GetFixtureB()->GetBody()->GetUserData();

				ComponentTransform* trans_pb1 = pb1->GetComponentTransform();
				ComponentTransform* trans_pb2 = pb2->GetComponentTransform();

				ScriptingBridgeGameObject* bridge_go1 = (ScriptingBridgeGameObject*)trans_pb1->GetOwner()->GetScriptingBridge();
				ScriptingBridgeGameObject* bridge_go2 = (ScriptingBridgeGameObject*)trans_pb2->GetOwner()->GetScriptingBridge();

				bridge_go2->CallOnCollisionStay(pb2);
				bridge_go2->CallOnCollisionStay(pb1);
			}
		}
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
		for (std::vector<PhysicsBody*>::iterator it = bodies.begin(); it != bodies.end(); ++it)
		{
			if ((*it) == body)
			{
				bodies.erase(it);
				break;
			}
		}

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
		vertices.push_back(float2(-10, 10));
		vertices.push_back(float2(-10, -10));
		vertices.push_back(float2(10, -10));
		vertices.push_back(float2(10, 10));

		((PhysicsShapePolygon*)ret)->SetVertices(vertices);

		break;
	}

	}

	if (ret != nullptr)
	{
		ret->SetFriction(0.3f);
		ret->SetDensity(1.0f);

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

std::vector<std::vector<float2>> ModulePhysics::TriangulateShape(const std::vector<float2>& shape)
{
	std::vector<std::vector<float2>> ret;

	if (shape.size() > 3)
	{
		using Point = std::array<float, 2>;
		std::vector<std::vector<Point>> polygon;
		std::vector<Point> polygon_points;

		for (std::vector<float2>::const_iterator it = shape.begin(); it != shape.end(); ++it)
		{
			float2 curr_point = (*it);

			polygon_points.push_back({ {curr_point.x, curr_point.y} });
		}

		if (polygon_points.size() > 3)
		{
			polygon.push_back(polygon_points);

			std::vector<int> indices = mapbox::earcut<int>(polygon);

			int indice_counter = 0;
			std::vector<float2> curr_shape;
			for (std::vector<int>::reverse_iterator it = indices.rbegin(); it != indices.rend(); ++it)
			{
				curr_shape.push_back(shape[(*it)]);

				if (indice_counter == 2)
				{
					indice_counter = -1;
					ret.push_back(curr_shape);
					curr_shape.clear();
				}

				++indice_counter;
			}
		}
		else
		{
			ret.push_back(shape);
		}
	}
	else
	{
		ret.push_back(shape);
	}

	return ret;
}

bool ModulePhysics::GetTriangleIsValid(const std::vector<float2>& triangle)
{
	bool ret = true;

	if (triangle.size() == 3)
	{
		float length_1 = std::abs(triangle[0].Distance(triangle[1]));
		float length_2 = std::abs(triangle[1].Distance(triangle[2]));
		float length_3 = std::abs(triangle[2].Distance(triangle[0]));

		length_1 = PIXELS_TO_METERS(length_1);
		length_2 = PIXELS_TO_METERS(length_2);
		length_3 = PIXELS_TO_METERS(length_3);

		float s = (length_1 + length_2 + length_3) * 0.5f;

		float area = std::sqrtf(s * (s - length_1) * (s - length_2) * (s - length_3));

		if (area < MIN_METERS_SIZE * MIN_METERS_SIZE)
			ret = false;

		if (length_1 < MIN_METERS_SIZE)
			ret = false;

		if (length_2 < MIN_METERS_SIZE)
			ret = false;

		if (length_3 < MIN_METERS_SIZE)
			ret = false;
	}

	return ret;
}

float2 ModulePhysics::GetLineClosestPointToPoint(const float2 & line_p1, const float2 & line_p2, const float2 & point)
{
	float2 ret = float2::zero;

	float2 ab = line_p2 - line_p1;
	float2 ap = point - line_p1;

	float lengthSqrAB = (ab.x * ab.x) + (ab.y * ab.y);
	float t = ((ap.x * ab.x) + (ap.y * ab.y)) / lengthSqrAB;

	if (t < 0)
		t = 0;

	if (t > 1)
		t = 1;

	ret = line_p1 + t * ab;

	return ret;
}

int ModulePhysics::GetPhysicsBodiesCount() const
{
	return bodies.size();
}

int ModulePhysics::GetPhysicsShapesCount() const
{
	return shapes.size();
}

void ModulePhysics::DestroyAllPhysicsBodies()
{
	for (std::vector<PhysicsBody*>::iterator it = bodies.begin(); it != bodies.end(); ++it)
	{
		RELEASE(*it);
	}

	bodies.clear();
}

void ModulePhysics::DestroyAllPhysicsShapes()
{
	for (std::vector<PhysicsShape*>::iterator it = shapes.begin(); it != shapes.end(); ++it)
	{
		RELEASE(*it);
	}

	shapes.clear();
}
