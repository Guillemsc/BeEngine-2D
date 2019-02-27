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
	if (attached_body != nullptr && fixtures.size() == 0)
	{
		b2FixtureDef b2fixture;
		b2fixture.density = 0;
		b2fixture.shape = &b2polygon_shape;

		b2Fixture* polygon = attached_body->b2body->CreateFixture(&b2fixture);
		fixtures.push_back(polygon);
	}
}

void PhysicsShapePolygon::GuizmoDraw()
{
	if (attached_body != nullptr)
	{	
		if (vertices.size() > 2)
		{
			float2 prev, v;

			int counter = 0;
			for (std::vector<float2>::iterator it = vertices.begin(); it != vertices.end(); ++it)
			{
				b2Vec2 b2v = b2Vec2(PIXELS_TO_METERS((*it).x), PIXELS_TO_METERS((*it).y));

				b2v = attached_body->b2body->GetWorldPoint(b2v);

				v = float2(b2v.x, b2v.y);

				if (counter > 0)
				{
					float2 p1 = float2(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y));
					float2 p2 = float2(METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y));

					App->scene_renderer->line_renderer->DrawLine(p1, p2, float3(0, 1, 0), 1, 1);
				}

				prev = v;

				++counter;
			}

			b2Vec2 b2v = b2Vec2(PIXELS_TO_METERS(vertices[0].x), PIXELS_TO_METERS(vertices[0].y));
			b2v = attached_body->b2body->GetWorldPoint(b2v);

			v = float2(b2v.x, b2v.y);

			float2 p1 = float2(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y));
			float2 p2 = float2(METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y));

			App->scene_renderer->line_renderer->DrawLine(p1, p2, float3(0, 1, 0), 1, 1);
		}
	}
}

void PhysicsShapePolygon::SetVertices(const std::vector<float2>& set)
{
	b2Vec2* p = new b2Vec2[set.size()];

	vertices = set;

	int counter = 0;
	for (std::vector<float2>::const_iterator it = set.begin(); it != set.end(); ++it)
	{
		float2 point = (*it);

		p[counter++] = b2Vec2(PIXELS_TO_METERS(point.x), PIXELS_TO_METERS(point.y));
	}

	b2polygon_shape.Set(p, set.size());
}

std::vector<float2> PhysicsShapePolygon::GetVertices() const
{
	return vertices;
}
