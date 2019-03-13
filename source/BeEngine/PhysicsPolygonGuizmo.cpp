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
#include "ModuleCamera.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

void PhysicsPolygonGuizmo::Start()
{
}

void PhysicsPolygonGuizmo::Render(float relative_size, const float2& mouse_pos)
{
	if (polygon_editing != nullptr)
	{
		PhysicsShapePolygon* shape = polygon_editing->GetPhysicsShapePolygon();

		if (shape != nullptr)
		{
			PhysicsBody* body = shape->GetAttachedBody();

			if (body != nullptr)
			{
				float2 size = float2(15 * relative_size, 15 * relative_size);

				App->scene_renderer->quad_renderer->SetZPos(App->scene_renderer->layer_space_guizmos.GetLayerValue(101));

				float2 mouse_closest_point = float2::inf;
				int closest_point_index = -1;

				bool over_existent_point = false;

				int counter = 0;
				for (std::vector<float2>::iterator it = polygon_points.begin(); it != polygon_points.end(); ++it, ++counter)
				{
					GuizmoHandler* handler = GetHandler(counter);

					float2 curr_pos = body->LocalPointToWorldPoint((*it));

					float3 colour = float3(0, 1, 0.5);

					if (handler->GetPressed() && !over_existent_point)
					{
						(*it) += float2(App->input->GetMouseXMotion() * relative_size, -App->input->GetMouseYMotion() * relative_size);

						over_existent_point = true;
					}
					
					if ((handler->GetHovered() && !over_existent_point) || handler->GetPressed())
					{
						colour = float3(247.0f / 255.0f, 188.0f / 255.0f, 43.0f / 255.0f);

						over_existent_point = true;
					}

					handler->SetTransfroms(curr_pos, size);

					App->scene_renderer->quad_renderer->DrawQuad(curr_pos, size * 1, colour, 1);

					float2 next_pos = float2::zero;

					if (counter >= polygon_points.size() - 1)
						next_pos = body->LocalPointToWorldPoint(polygon_points[0]);
					else
						next_pos = body->LocalPointToWorldPoint(polygon_points[counter + 1]);

					float thickness = 2.5f * relative_size;

					float2 colosest_point_to_line = App->physics->GetLineClosestPointToPoint(curr_pos, next_pos, mouse_pos);

					float distance_mouse_curr = std::abs(mouse_pos.Distance(colosest_point_to_line));
					float distance_mouse_closest = std::abs(mouse_pos.Distance(mouse_closest_point));

					if (distance_mouse_curr < distance_mouse_closest)
					{
						mouse_closest_point = colosest_point_to_line;
						closest_point_index = counter;
					}
					
					App->scene_renderer->line_renderer->DrawLine(curr_pos, next_pos, float3(0.8f, 1, 0), 1, thickness);
				}

				if (!over_existent_point)
				{
					float3 colour = float3(1, 0.3f, 0.5f);

					if(add_point_handler->GetHovered())
						colour = float3(1, 0.5f, 0.7f);

					App->scene_renderer->quad_renderer->DrawQuad(mouse_closest_point, size * 0.8f, colour, 1);

					add_point_handler->SetActive(true);
					add_point_handler->SetTransfroms(mouse_closest_point, size * 0.8f);

					if (add_point_handler->GetPressed() && !add_point)
					{
						add_point = true;

						if (closest_point_index > -1)
							AddPoint(closest_point_index + 1, mouse_closest_point);

						add_point_handler->SetActive(false);
					}
				}
				else
				{
					add_point_handler->SetActive(false);
				}

				if (!add_point_handler->GetPressed())
					add_point = false;
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

void PhysicsPolygonGuizmo::SetPolygonPoint(int index, const float2 & point)
{
	if(polygon_points.size() > index)
	{
		polygon_points[index] = point;
	}
}

std::vector<float2> PhysicsPolygonGuizmo::GetPolygonPoints() const
{
	return polygon_points;
}

void PhysicsPolygonGuizmo::RemovePoint(int index)
{
	int counter = 0;
	for (std::vector<float2>::iterator it = polygon_points.begin(); it != polygon_points.end(); ++it, ++counter)
	{
		if (counter == index)
		{
			polygon_points.erase(it);
			break;
		}
	}
}

void PhysicsPolygonGuizmo::RebuildHandlers()
{
	DestroyAllHandlers();

	for (std::vector<float2>::iterator it = polygon_points.begin(); it != polygon_points.end(); ++it)
	{
		AddHandler();
	}

	add_point_handler = AddHandler();
}

void PhysicsPolygonGuizmo::AddPoint(int index, const float2 & point)
{
	if (index == polygon_points.size())
	{
		polygon_points.push_back(point);
	}
	else
	{
		int counter = 0;
		for (std::vector<float2>::iterator it = polygon_points.begin(); it != polygon_points.end(); ++it, ++counter)
		{
			if (counter == index)
			{
				polygon_points.insert(it, point);
				break;
			}
		}
	}

	RebuildHandlers();
}
