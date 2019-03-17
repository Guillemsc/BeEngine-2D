#include "CanvasItemGuizmo.h"

void CanvasItemGuizmo::Start()
{
}

void CanvasItemGuizmo::Render(float relative_size, const float2 & mouse_pos)
{
	if (canvas_item_editing != nullptr)
	{

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
