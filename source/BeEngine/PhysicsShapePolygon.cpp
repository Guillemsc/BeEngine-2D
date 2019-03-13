#include "PhysicsShapePolygon.h"
#include "PhysicsBody.h"
#include "LineRenderer.h"
#include "ModuleSceneRenderer.h"
#include "App.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

PhysicsShapePolygon::PhysicsShapePolygon() : PhysicsShape(PhysicsShapeType::PHYSICS_SHAPE_POLYGON)
{
}

void PhysicsShapePolygon::CreateFixture()
{
	DestroyFixture();

	if (attached_body != nullptr)
	{
		b2FixtureDef b2fixture;
		b2fixture.density = 0;
		b2fixture.isSensor = is_sensor;

		for (std::vector<b2PolygonShape>::iterator it = b2polygon_shapes.begin(); it != b2polygon_shapes.end(); ++it)
		{
			b2fixture.shape = &(*it);

			b2Fixture* fix = attached_body->b2body->CreateFixture(&b2fixture);
			fixtures.push_back(fix);
		}
	}
}

void PhysicsShapePolygon::DestroyFixture()
{
	for (std::vector<b2Fixture*>::iterator it = fixtures.begin(); it != fixtures.end(); ++it)
	{
		attached_body->b2body->DestroyFixture((*it));
	}

	fixtures.clear();
}

void PhysicsShapePolygon::SetVertices(const std::vector<float2>& set)
{
	b2polygon_shapes.clear();

	vertices = set;

	bool can_create_shape = true;

	if (set.size() < 3)
	{
		not_enough_vertices = true;
		can_create_shape = false;
	}
	else
		not_enough_vertices = false;

	std::vector<std::vector<float2>> shapes = App->physics->TriangulateShape(set);

	triangles.clear();
	valid = true;

	for (std::vector<std::vector<float2>>::iterator it = shapes.begin(); it != shapes.end(); ++it)
	{
		std::vector<float2> shape = (*it);

		triangles.push_back(shape);

		if(valid)
			valid = App->physics->GetTriangleIsValid(shape);

		if (!valid)
		{
			can_create_shape = false;
		}
	}

	if (can_create_shape)
	{
		for (std::vector<std::vector<float2>>::iterator it = shapes.begin(); it != shapes.end(); ++it)
		{
			std::vector<float2> shape = (*it);

			b2Vec2* p = new b2Vec2[shape.size()];

			int counter = 0;
			for (std::vector<float2>::const_iterator sh = shape.begin(); sh != shape.end(); ++sh)
			{
				float2 point = (*sh);

				p[counter] = b2Vec2(PIXELS_TO_METERS(point.x), PIXELS_TO_METERS(point.y));

				++counter;
			}

			if (valid)
			{
				b2PolygonShape b2polygon_shape;
				b2polygon_shape.Set(p, shape.size());

				b2polygon_shapes.push_back(b2polygon_shape);
			}

			RELEASE_ARRAY(p);
		}

		CreateFixture();
	}
	else
	{
		DestroyFixture();
	}
}

std::vector<float2> PhysicsShapePolygon::GetVertices() const
{
	return vertices;
}

bool PhysicsShapePolygon::GetShapeValid() const
{
	return valid;
}

bool PhysicsShapePolygon::GetNotEnoughVertices() const
{
	return not_enough_vertices;
}
