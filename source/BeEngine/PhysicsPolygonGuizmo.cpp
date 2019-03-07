#include "PhysicsPolygonGuizmo.h"
#include "ModuleSceneRenderer.h"
#include "LineRenderer.h"
#include "QuadRenderer.h"
#include "App.h"
#include "ComponentPolygonCollider.h"
#include "PhysicsShapePolygon.h"
#include "PhysicsBody.h"
#include "ModuleInput.h"
#include "ModuleGuizmo.h"
#include "PositionGuizmo.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

void PhysicsPolygonGuizmo::Start()
{
}

void PhysicsPolygonGuizmo::Render(float relative_size)
{
	if (polygon_editing != nullptr)
	{
		PhysicsShapePolygon* shape = polygon_editing->GetPhysicsShapePolygon();

		if (shape != nullptr)
		{
			PhysicsBody* body = shape->GetAttachedBody();

			if (body != nullptr)
			{
				float2 size = float2(10 * relative_size, 10 * relative_size);

				App->scene_renderer->quad_renderer->SetZPos(App->scene_renderer->layer_space_guizmos.GetLayerValue(101));

				float2 quad_size = float2(10 * relative_size, 10 * relative_size);

				int counter = 0;
				for (std::vector<float2>::iterator it = polygon_points.begin(); it != polygon_points.end(); ++it, ++counter)
				{
					GuizmoHandler* handler = GetHandler(counter);

					float2 curr_pos = body->LocalPointToWorldPoint((*it));
					float2 size = float2(10 * relative_size, 10 * relative_size);

					float3 colour = float3(0, 1, 0.5);

					if (handler->GetPressed())
						(*it) += float2(App->input->GetMouseXMotion() * relative_size, -App->input->GetMouseYMotion() * relative_size);

					if (handler->GetHovered())
						colour = float3(247.0f / 255.0f, 188.0f / 255.0f, 43.0f / 255.0f);

					handler->SetTransfroms(curr_pos, size);

					App->scene_renderer->quad_renderer->DrawQuad(curr_pos, size, colour, 1);

					float2 next_pos = float2::zero;

					if (counter >= polygon_points.size() - 1)
						next_pos = body->LocalPointToWorldPoint(polygon_points[0]);
					else
						next_pos = body->LocalPointToWorldPoint(polygon_points[counter + 1]);

					float thickness = 2.5f * relative_size;

					App->scene_renderer->line_renderer->DrawLine(curr_pos, next_pos, float3(0.8f, 1, 0), 1, thickness);
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

		App->guizmo->position_guizmo->SetEnabled(false);

		RebuildHandlers();
	}
}

std::vector<float2> PhysicsPolygonGuizmo::FinishEditing(ComponentPolygonCollider * editing)
{
	if (polygon_editing == editing)
	{
		PhysicsShapePolygon* shape = polygon_editing->GetPhysicsShapePolygon();

		shape->SetVertices(polygon_points);

		App->guizmo->position_guizmo->SetEnabled(true);

		polygon_editing = nullptr;
	}

	return polygon_points;
}

void PhysicsPolygonGuizmo::StopEditing(ComponentPolygonCollider * editing)
{
	if (polygon_editing == editing)
	{
		App->guizmo->position_guizmo->SetEnabled(true);

		polygon_editing = nullptr;
		polygon_points.clear();
	}
}

ComponentPolygonCollider * PhysicsPolygonGuizmo::GetEditingComponent() const
{
	return polygon_editing;
}

GameObject* PhysicsPolygonGuizmo::GetEditingGameObject() const
{
	GameObject* ret = nullptr;

	if (polygon_editing != nullptr)
	{
		ret = polygon_editing->GetOwner();
	}

	return ret;
}

void PhysicsPolygonGuizmo::RebuildHandlers()
{
	DestroyAllHandlers();

	for (std::vector<float2>::iterator it = polygon_points.begin(); it != polygon_points.end(); ++it)
	{
		AddHandler();
	}
}
