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

	if (attached_body != nullptr && fixtures.size() == 0)
	{
		b2FixtureDef b2fixture;
		b2fixture.density = 0;
		b2fixture.shape = &b2polygon_shape;
		b2fixture.isSensor = is_sensor;

		polygon = attached_body->b2body->CreateFixture(&b2fixture);
		fixtures.push_back(polygon);
	}
}

void PhysicsShapePolygon::DestroyFixture()
{
	if (polygon != nullptr)
	{
		if (attached_body != nullptr && fixtures.size() > 0)
		{
			attached_body->b2body->DestroyFixture(polygon);

			fixtures.clear();
		}
	}
}

void PhysicsShapePolygon::RenderGuizmo()
{
	App->scene_renderer->line_renderer->SetZPos(App->scene_renderer->layer_space_guizmos.GetLayerValue(99));

	if (attached_body != nullptr)
	{	
		if (vertices.size() > 2)
		{
			if (polygon != nullptr)
			{
				b2PolygonShape* polygonShape = (b2PolygonShape*)polygon->GetShape();
				int32 count = polygonShape->GetVertexCount();
				b2Vec2 prev, v;

				for (int32 i = 0; i < count; ++i)
				{
					v = attached_body->b2body->GetWorldPoint(polygonShape->GetVertex(i));

					if (i > 0)
					{
						App->scene_renderer->line_renderer->DrawLine(float2(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y)), float2(METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y)), float3(0, 1, 0), 1, 0.5f);
					}

					prev = v;
				}

				v = attached_body->b2body->GetWorldPoint(polygonShape->GetVertex(0));

				App->scene_renderer->line_renderer->DrawLine(float2(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y)), float2(METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y)), float3(0, 1, 0), 1, 0.5f);

				// -----------
				/*float2 prev, v;

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

						App->scene_renderer->line_renderer->DrawLine(p1, p2, float3(0, 1, 0), 1, 0.5f);
					}

					prev = v;

					++counter;
				}

				b2Vec2 b2v = b2Vec2(PIXELS_TO_METERS(vertices[0].x), PIXELS_TO_METERS(vertices[0].y));
				b2v = attached_body->b2body->GetWorldPoint(b2v);

				v = float2(b2v.x, b2v.y);

				float2 p1 = float2(METERS_TO_PIXELS(prev.x), METERS_TO_PIXELS(prev.y));
				float2 p2 = float2(METERS_TO_PIXELS(v.x), METERS_TO_PIXELS(v.y));

				App->scene_renderer->line_renderer->DrawLine(p1, p2, float3(0, 1, 0), 1, 0.5f);*/
			}
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

		p[counter] = b2Vec2(point.x, point.y);

		++counter;
	}

	b2polygon_shape.Set(p, set.size());

	b2polygon_shape.SetAsBox(PIXELS_TO_METERS(20), PIXELS_TO_METERS(20));

	CreateFixture();
}

std::vector<float2> PhysicsShapePolygon::GetVertices() const
{
	return vertices;
}
