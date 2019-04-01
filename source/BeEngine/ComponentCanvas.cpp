#include "ComponentCanvas.h"
#include "App.h"
#include "GameObject.h"
#include "ComponentTransfrom.h"
#include "ModuleCamera.h"
#include "ModuleEditor.h"
#include "GameWindow.h"
#include "ComponentCamera.h"
#include "ModuleGameObject.h"
#include "ModuleEvent.h"
#include "ScriptingBridgeComponentCanvas.h"

ComponentCanvas::ComponentCanvas() 
	: GameObjectComponent(new ScriptingBridgeComponentCanvas(this),
		"Canvas", ComponentType::COMPONENT_TYPE_CANVAS, ComponentGroup::UI, true)
{
}

ComponentCanvas::~ComponentCanvas()
{
}

void ComponentCanvas::EditorDraw()
{
	
}

void ComponentCanvas::Start()
{
	InitBeObject();
}

void ComponentCanvas::Update()
{
	UpdateCanvasPosition();

	UpdateCanvasLayout();
}

void ComponentCanvas::CleanUp()
{
	CleanUpBeObject();
}

void ComponentCanvas::OnSaveAbstraction(DataAbstraction & abs)
{
}

void ComponentCanvas::OnLoadAbstraction(DataAbstraction & abs)
{
}

void ComponentCanvas::OnChildAdded(GameObject * child)
{
}

void ComponentCanvas::OnChildRemoved(GameObject * child)
{
}

void ComponentCanvas::OnParentChanged(GameObject * new_parent)
{
}

void ComponentCanvas::RenderGuizmos(float relative_size)
{
}

float2 ComponentCanvas::GetCanvasCenter() const
{
	float2 ret = float2::zero;

	ret = GetOwner()->transform->GetPosition();

	return ret;
}

float2 ComponentCanvas::GetPositionFromAnchorPoint(const float2& anchor_point)
{
	float2 ret = float2::zero;

	float2 point = anchor_point;

	if (point.x > 1)
		point.x = 1;

	if (point.x < -1)
		point.x = -1;

	if (point.y > 1)
		point.y = 1;

	if (point.y < -1)
		point.y = -1;

	Camera2D* game_cam = App->camera->GetGameCamera();

	if (game_cam != nullptr)
	{
		float2 win_size = game_cam->GetViewportSize();
		float2 half_win_size = win_size * 0.5f;

		float2 center = GetCanvasCenter();

		if (point.x > 0)
		{
			ret.x = (half_win_size.x / 1.0f) * point.x;
		}
		else if (point.x < 0)
		{
			ret.x = -((half_win_size.x / 1.0f) * std::abs(point.x));
		}

		if (point.y > 0)
		{
			ret.y = (half_win_size.y / 1.0f) * point.y;
		}
		else if (point.y < 0)
		{
			ret.y = -((half_win_size.y / 1.0f) * std::abs(point.y));
		}

		ret += center;
	}

	return ret;
}

float2 ComponentCanvas::GetAnchorPointFromPosition(const float2 & position)
{
	float2 ret = float2::zero;

	Camera2D* game_cam = App->camera->GetGameCamera();

	if (game_cam != nullptr)
	{
		float2 win_size = game_cam->GetViewportSize();
		float2 half_win_size = win_size * 0.5f;

		float2 center = GetCanvasCenter();

		float2 center_offset = center - position;

		if (center_offset.x > 0 && half_win_size.x > 0)
		{
			ret.x = -((1 * std::abs(center_offset.x)) / half_win_size.x);
		}
		else if (center_offset.x < 0 && half_win_size.x > 0)
		{
			ret.x = (1 * std::abs(center_offset.x)) / half_win_size.x;
		}

		if (center_offset.y > 0 && half_win_size.y > 0)
		{
			ret.y = -((1 * std::abs(center_offset.y)) / half_win_size.y);
		}
		else if (center_offset.y < 0 && half_win_size.y > 0)
		{
			ret.y = (1 * std::abs(center_offset.y)) / half_win_size.y;
		}
	}

	return ret;
}

void ComponentCanvas::UpdateCanvasPosition()
{
	Camera2D* game_camera = App->camera->GetGameCamera();

	if (game_camera != nullptr)
	{
		ComponentCamera* comp_camera = game_camera->GetComponentCamera();

		if (comp_camera != nullptr)
		{
			float4x4 world_trans = comp_camera->GetOwner()->transform->GetWorldTransform();

			GetOwner()->transform->SetWorldTransform(world_trans);
		}
	}
}

void ComponentCanvas::UpdateCanvasLayout()
{
	std::vector<GameObject*> childs = GetOwner()->GetChilds();

	for (std::vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
	{
		GameObject* curr_go = (*it);

		curr_go->transform->RecalculateCanvasLayout();
	}
}
