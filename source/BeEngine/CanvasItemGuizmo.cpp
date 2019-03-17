#include "CanvasItemGuizmo.h"
#include "ComponentTransfrom.h"
#include "ModuleSceneRenderer.h"
#include "App.h"
#include "QuadRenderer.h"
#include "LineRenderer.h"
#include "ComponentCanvas.h"
#include "ModuleCamera.h"
#include "ComponentCamera.h"
#include "ModuleGuizmo.h"
#include "PositionGuizmo.h"
#include "ModuleInput.h"

void CanvasItemGuizmo::Start()
{
	AddHandler();
}

void CanvasItemGuizmo::Render(float relative_size, const float2 & mouse_pos)
{
	if (canvas_item_editing != nullptr)
	{
		GuizmoHandler* anchor_guizmo = GetHandler(CanvasItemGuizmoHandlers::ANCHOR);

		Camera2D* cam = App->camera->GetGameCamera();

		if (cam != nullptr)
		{
			ComponentCamera* comp_cam = cam->GetComponentCamera();

			if (comp_cam != nullptr)
				comp_cam->RenderGuizmos(relative_size);
		}

		ComponentCanvas* parent_used_canvas = canvas_item_editing->GetParentUsedCanvas();

		float2 anchor_pos = canvas_item_editing->GetAnchorPos();

		if (parent_used_canvas != nullptr)
		{
			float2 size = float2(20.5f * relative_size, 20.5f * relative_size);

			float line_lenght = 10 * relative_size;
			float line_thickness = 3.0f * relative_size;

			float3 anchor_base_colour = float3(1, 1, 1);
			float3 anchor_hovered_colour = float3(247.0f / 255.0f, 188.0f / 255.0f, 43.0f / 255.0f);
			float3 anchor_color = anchor_base_colour;

			float3 marker_lines_colour = float3(1, 1, 1);

			App->scene_renderer->quad_renderer->SetZPos(App->scene_renderer->layer_space_guizmos.GetLayerValue(99));

			float2 anchor_world_pos = parent_used_canvas->GetPositionFromAnchorPoint(anchor_pos);
			float2 game_object_pos = canvas_item_editing->GetPosition();
			float2 game_object_anchor_offset = game_object_pos - anchor_world_pos;

			anchor_guizmo->SetTransfroms(anchor_world_pos, float2(line_lenght * 2, line_lenght * 2));

			if (anchor_guizmo->GetHovered())
				anchor_color = anchor_hovered_colour;

			if (anchor_guizmo->GetPressed())
			{
				float2 movement = float2(App->input->GetMouseXMotion() * relative_size, -App->input->GetMouseYMotion() * relative_size);

				anchor_world_pos += movement;

				float2 new_anchor = parent_used_canvas->GetAnchorPointFromPosition(anchor_world_pos);

				canvas_item_editing->SetAnchorPos(new_anchor);
			}

			float2 end_pos1 = float2(anchor_world_pos.x - line_lenght, anchor_world_pos.y - line_lenght);
			App->scene_renderer->line_renderer->DrawLine(anchor_world_pos, end_pos1, anchor_color, 1, line_thickness);

			float2 end_pos2 = float2(anchor_world_pos.x + line_lenght, anchor_world_pos.y - line_lenght);
			App->scene_renderer->line_renderer->DrawLine(anchor_world_pos, end_pos2, anchor_color, 1, line_thickness);

			float2 end_pos3 = float2(anchor_world_pos.x + line_lenght, anchor_world_pos.y + line_lenght);
			App->scene_renderer->line_renderer->DrawLine(anchor_world_pos, end_pos3, anchor_color, 1, line_thickness);

			float2 end_pos4 = float2(anchor_world_pos.x - line_lenght, anchor_world_pos.y + line_lenght);
			App->scene_renderer->line_renderer->DrawLine(anchor_world_pos, end_pos4, anchor_color, 1, line_thickness);

			float2 end_pos5 = float2(anchor_world_pos.x + game_object_anchor_offset.x, anchor_world_pos.y);
			App->scene_renderer->line_renderer->DrawLine(anchor_world_pos, end_pos5, marker_lines_colour, 1, line_thickness * 0.5f);

			float2 end_pos6 = float2(anchor_world_pos.x, anchor_world_pos.y + game_object_anchor_offset.y);
			App->scene_renderer->line_renderer->DrawLine(anchor_world_pos, end_pos6, marker_lines_colour, 1, line_thickness * 0.5f);

			float2 end_pos7 = float2(game_object_pos.x, game_object_pos.y - game_object_anchor_offset.y);
			App->scene_renderer->line_renderer->DrawLine(game_object_pos, end_pos7, marker_lines_colour, 1, line_thickness * 0.5f);

			float2 end_pos8 = float2(game_object_pos.x - game_object_anchor_offset.x, game_object_pos.y);
			App->scene_renderer->line_renderer->DrawLine(game_object_pos, end_pos8, marker_lines_colour, 1, line_thickness * 0.5f);
		}
	}
}

void CanvasItemGuizmo::StartEditing(ComponentTransform * to_edit)
{
	if (to_edit != nullptr)
	{
		canvas_item_editing = to_edit;
	}
}

void CanvasItemGuizmo::StopEditing(ComponentTransform * stop_edit)
{
	if (canvas_item_editing == stop_edit)
	{
		canvas_item_editing = nullptr;
	}
}

ComponentTransform * CanvasItemGuizmo::GetEditingComponent() const
{
	return nullptr;
}

void CanvasItemGuizmo::CreateHandlers()
{
}

void CanvasItemGuizmo::DestroyHandlers()
{
}
