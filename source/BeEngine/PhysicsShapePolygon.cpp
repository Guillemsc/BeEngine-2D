#include "PhysicsShapePolygon.h"
#include "PhysicsBody.h"
#include "LineRenderer.h"
#include "ModuleSceneRenderer.h"
#include "App.h"

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

	for (std::vector<std::vector<float2>>::iterator it = shapes.begin(); it != shapes.end(); ++it)
	{
		std::vector<float2> shape = (*it);

		triangles.push_back(shape);

		too_small = App->physics->GetTriangleIsTooSmall(shape);

		if (too_small)
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

			if (!too_small)
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

void PhysicsShapePolygon::SetAmoutVertices(int set)
{
	if (set > vertices.size())
	{
		std::vector<float2> new_vertices = vertices;

		int to_add = set - vertices.size();

		while (to_add <= 0)
		{
			if (vertices.size() == 0)
			{
				new_vertices.push_back(float2(0, 0));
			}
			else
			{
				new_vertices.push_back(new_vertices[new_vertices.size() - 1]);
			}

			--to_add;
		}

		SetVertices(new_vertices);
	}
	else if (set < vertices.size())
	{
		std::vector<float2> new_vertices;

		int counter = 0;
		for (std::vector<float2>::iterator it = vertices.begin(); it != vertices.end(); ++it, ++counter)
		{
			new_vertices.push_back((*it));

			if (counter >= set - 1)
			{
				break;
			}
		}

		SetVertices(new_vertices);
	}
}

bool PhysicsShapePolygon::GetShapeTooSmall() const
{
	return too_small;
}

bool PhysicsShapePolygon::GetNotEnoughVertices() const
{
	return not_enough_vertices;
}
