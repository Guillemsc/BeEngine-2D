#include "PhysicsPolygonGuizmo.h"
#include "ModuleSceneRenderer.h"
#include "LineRenderer.h"
#include "QuadRenderer.h"
#include "App.h"
#include "ComponentPolygonCollider.h"
#include "PhysicsShapePolygon.h"
#include "PhysicsBody.h"

void PhysicsPolygonGuizmo::Start()
{
}

void PhysicsPolygonGuizmo::Render(float relative_size)
{
	if (polygon_editing != nullptr)
	{
		PhysicsShape* shape = polygon_editing->GetPhysicsShapePolygon();

		if (shape != nullptr)
		{
			PhysicsBody* body = shape->GetAttachedBody();

			if (body != nullptr)
			{
				App->scene_renderer->line_renderer->SetZPos(App->scene_renderer->layer_space_guizmos.GetLayerValue(100));
				App->scene_renderer->quad_renderer->SetZPos(App->scene_renderer->layer_space_guizmos.GetLayerValue(101));

				float2 quad_size = float2(10 * relative_size, 10 * relative_size);
				float thickness = 2 * relative_size;

				int counter = 0;
				for (std::vector<float2>::iterator it = polygon_points.begin(); it != polygon_points.end(); ++it, ++counter)
				{
					float2 curr_pos = body->LocalPointToWorldPoint((*it));
					float2 size = float2(10 * relative_size, 10 * relative_size);

					App->scene_renderer->quad_renderer->DrawQuad(curr_pos, size, float3(1, 1, 0), 1);

					float2 next_pos = float2::zero;

					if (counter >= polygon_points.size() - 1)
						next_pos = body->LocalPointToWorldPoint(polygon_points[0]);
					else
						next_pos = body->LocalPointToWorldPoint(polygon_points[counter + 1]);
					
					App->scene_renderer->line_renderer->DrawLine(curr_pos, next_pos, float3(0, 1, 0), 1, thickness);
				}
			}
		}
	}
}

void PhysicsPolygonGuizmo::StartEditing(ComponentPolygonCollider * editing)
{
	if (editing != nullptr)
	{
		polygon_editing = editing;

		polygon_points = polygon_editing->GetPhysicsShapePolygon()->GetVertices();
	}
}

std::vector<float2> PhysicsPolygonGuizmo::FinishEditing(ComponentPolygonCollider * editing)
{
	if (polygon_editing == editing)
	{
		polygon_editing = nullptr;
	}

	return polygon_points;
}

void PhysicsPolygonGuizmo::StopEditing(ComponentPolygonCollider * editing)
{
	if (polygon_editing == editing)
	{
		polygon_editing = nullptr;
		polygon_points.clear();
	}
}

ComponentPolygonCollider * PhysicsPolygonGuizmo::GetEditingComponent() const
{
	return polygon_editing;
}
