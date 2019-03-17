#include "ComponentTransfrom.h"
#include "GameObject.h"
#include "imgui.h"
#include "ModuleJson.h"
#include "ScriptingBridgeComponentTransform.h"
#include "ModuleScripting.h"
#include "App.h"
#include "PhysicsBody.h"
#include "ModuleGameObject.h"
#include "ModuleSceneRenderer.h"
#include "QuadRenderer.h"
#include "ComponentCanvas.h"
#include "ModuleGuizmo.h"
#include "CanvasItemGuizmo.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

ComponentTransform::ComponentTransform() 
	: GameObjectComponent("Transform", ComponentType::COMPONENT_TYPE_TRANSFORM, ComponentGroup::TRANSFORMATIONS, true, false)
{

}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::EditorDraw()
{
	float2 world_position = GetPosition();

	if (used_canvas_comp == nullptr)
	{
		float2 position = GetLocalPosition();
		float rotation = GetLocalRotationDegrees();
		float2 scale = GetLocalScale();

		if (parent_used_canvas != nullptr)
		{
			if (ImGui::DragFloat2("Anchor", (float*)&anchor_pos, 0.1f))
				SetAnchorPos(anchor_pos);

			if (ImGui::DragFloat2("Anchor offset", (float*)&anchor_offset_pos, 0.1f))
				SetAnchorOffsetPos(anchor_offset_pos);
		}
		else
		{
			if (ImGui::DragFloat2("Position", (float*)&position, 0.1f))
				SetLocalPosition(position);
		}

		if (ImGui::DragFloat("Rotation", (float*)&rotation, 0.1f))
			SetLocalRotationDegrees(rotation);

		if (ImGui::DragFloat2("Scale", (float*)&scale, 0.1f))
			SetLocalScale(scale);

		ImGui::Checkbox("Keep scale ratio", &keep_scale_ratio);
	}
	else
	{
		ImGui::Text("Position is controled by the UI Canvas");
	}

	ImGui::Text("World Positions: ");

	std::string x_world = "x: " + std::to_string(world_position.x);
	std::string y_world = "y: " + std::to_string(world_position.y);

	ImGui::Text(x_world.c_str());
	ImGui::SameLine();
	ImGui::Text(y_world.c_str());

}

void ComponentTransform::Start()
{
	local_transform = float4x4::identity;
	local_transform[0][3] = 1;
	local_transform[1][3] = 1;
	local_transform[2][3] = 1;

	world_transform = float4x4::identity;
	world_transform[0][3] = 1;
	world_transform[1][3] = 1;
	world_transform[2][3] = 1;

	scripting_bridge = new ScriptingBridgeComponentTransform(this);
	App->scripting->AddScriptingBridgeObject(scripting_bridge);

	base_physics_body = App->physics->CreatePhysicsBody();
	base_physics_body->SetType(PhysicsBodyType::PHYSICS_BODY_KINEMATIC);
	base_physics_body->SetComponentTransform(this);

	UpdateLocalTransformFromValues();
}

void ComponentTransform::Update()
{
	UpdatePhysicsMovement();
	FindParentUsedCanvas();
}

void ComponentTransform::CleanUp()
{
	App->physics->DestroyPhysicsBody(base_physics_body);

	App->scripting->DestroyScriptingBridgeObject(scripting_bridge);
}

void ComponentTransform::RenderGuizmos(float relative_size)
{

}

void ComponentTransform::OnSaveAbstraction(DataAbstraction & abs)
{
	abs.AddFloat2("anchor_pos", GetAnchorPos());
	abs.AddFloat2("position", GetPosition());
	abs.AddFloat2("scale", GetScale());
	abs.AddFloat("rotation", GetRotationDegrees());
	abs.AddBool("keep_ratio", keep_scale_ratio);
}

void ComponentTransform::OnLoadAbstraction(DataAbstraction & abs)
{
	keep_scale_ratio = abs.GetBool("keep_ratio");
	SetAnchorPos(abs.GetFloat2("anchor_pos"));
	SetPosition(abs.GetFloat2("position"));
	SetScale(abs.GetFloat2("scale"));
	SetRotationDegrees(abs.GetFloat("rotation"));
}

void ComponentTransform::OnDestroy()
{
	App->guizmo->canvas_item_guizmo->StopEditing(this);
}

void ComponentTransform::OnOwnerSelected()
{
	if (parent_used_canvas != nullptr)
		App->guizmo->canvas_item_guizmo->StartEditing(this);
}

void ComponentTransform::OnOwnerDeSelected()
{
	App->guizmo->canvas_item_guizmo->StopEditing(this);
}

void ComponentTransform::OnChildAdded(GameObject * child)
{
}

void ComponentTransform::OnChildRemoved(GameObject * child)
{
}

void ComponentTransform::OnParentChanged(GameObject * new_parent)
{
}

void ComponentTransform::OnAddComponent(GameObjectComponent * new_component)
{
	switch (new_component->GetType())
	{
	case ComponentType::COMPONENT_TYPE_PHYSICS_BODY:
	{
		base_physics_body->SetType(PhysicsBodyType::PHYSICS_BODY_DYNAMIC);
		base_physics_body->ClearForces();

		break;
	}

	case ComponentType::COMPONENT_TYPE_CANVAS:
	{
		used_canvas_comp = (ComponentCanvas*)new_component;

		break;
	}
	}
}

void ComponentTransform::OnRemoveComponent(GameObjectComponent * deleted)
{
	switch (deleted->GetType())
	{
	case ComponentType::COMPONENT_TYPE_PHYSICS_BODY:
	{
		base_physics_body->SetType(PhysicsBodyType::PHYSICS_BODY_KINEMATIC);
		base_physics_body->ClearForces();

		break;
	}

	case ComponentType::COMPONENT_TYPE_CANVAS:
	{
		used_canvas_comp = nullptr;

		break;
	}
	}
}

void ComponentTransform::SetLocalTransform(const float4x4 & local)
{
	local_transform = local;

	RecalculateWorldTransform();
}

void ComponentTransform::SetWorldTransform(const float4x4 & world)
{
	world_transform = world;

	RecalculateLocalTransform();
}

void ComponentTransform::SetLocalPosition(const float2 & pos)
{
	if(pos.x != local_pos.x || pos.y != local_pos.y)
	{
		local_pos = pos;

		UpdateLocalTransformFromValues();
	}
}

void ComponentTransform::SetLocalRotationDegrees(float rotation)
{
	rotation *= DEGTORAD;

	if (rotation != local_rotation)
	{
		local_rotation = rotation;

		UpdateLocalTransformFromValues();
	}
}

void ComponentTransform::SetLocalScale(const float2 & scale)
{
	bool update_scale = false;

	float2 to_set = scale;

	if (scale.x != local_scale.x)
	{
		if (keep_scale_ratio)
			to_set.y = scale.x;

		update_scale = true;
	}

	if (scale.y != local_scale.y)
	{
		if (keep_scale_ratio)
			to_set.x = scale.y;

		update_scale = true;
	}

	if (update_scale)
	{
		local_scale = to_set;

		if (local_scale.x <= 0)
			local_scale.x = 0.01f;

		if (local_scale.y <= 0)
			local_scale.y = 0.01f;

		UpdateLocalTransformFromValues();
	}
}

void ComponentTransform::SetPosition(const float2 & pos)
{	
	base_physics_body->SetPosition(pos);

	UpdateWorldTransformFromValues();
}

void ComponentTransform::SetRotationDegrees(float rotation)
{
	float world_rotation = base_physics_body->GetRotationDegrees();

	if (rotation != world_rotation)
	{
		base_physics_body->SetRotationDegrees(rotation);

		UpdateWorldTransformFromValues();
	}
}

void ComponentTransform::SetScale(const float2 & scale)
{
	bool update_scale = false;

	float2 to_set = scale;

	if (scale.x != world_scale.x)
	{
		if (keep_scale_ratio)
			to_set.y = scale.x;

		update_scale = true;
	}

	if (scale.y != world_scale.y)
	{
		if (keep_scale_ratio)
			to_set.x = scale.y;

		update_scale = true;
	}

	if (update_scale)
	{
		world_scale = to_set;

		if (world_scale.x <= 0)
			world_scale.x = 0.01f;

		if (world_scale.y <= 0)
			world_scale.y = 0.01f;

		UpdateWorldTransformFromValues();
	}
}

float2 ComponentTransform::GetLocalPosition()
{
	RecalculateLocalTransform();

	return local_pos;
}

float ComponentTransform::GetLocalRotationDegrees()
{
	RecalculateLocalTransform();

	return local_rotation * RADTODEG;
}

float2 ComponentTransform::GetLocalScale()
{
	RecalculateLocalTransform();

	return local_scale;
}

float2 ComponentTransform::GetPosition()
{
	float2 ret = base_physics_body->GetPosition();

	RecalculateWorldTransform();

	return ret;
}

float ComponentTransform::GetRotationDegrees()
{
	float ret = base_physics_body->GetRotationDegrees();

	RecalculateWorldTransform();

	return ret;
}

float2 ComponentTransform::GetScale()
{
	float2 ret = world_scale;

	RecalculateWorldTransform();

	return ret;
}

float4x4 ComponentTransform::GetLocalTransform()
{
	RecalculateLocalTransform();

	return local_transform;
}

float4x4 ComponentTransform::GetWorldTransform()
{
	RecalculateWorldTransform();

	return world_transform;
}

void ComponentTransform::SetAnchorPos(const float2& anchor)
{
	anchor_pos = anchor;

	if (anchor_pos.x > 1)
		anchor_pos.x = 1;

	if (anchor_pos.x < -1)
		anchor_pos.x = -1;

	if (anchor_pos.y > 1)
		anchor_pos.y = 1;

	if (anchor_pos.y < -1)
		anchor_pos.y = -1;

	SetAnchorOffsetPos(anchor_offset_pos);
}

void ComponentTransform::SetAnchorOffsetPos(const float2 & anchor_offset)
{
	anchor_offset_pos = anchor_offset;

	if (parent_used_canvas != nullptr && GetOwner()->GetParent() != nullptr)
	{
		if (parent_used_canvas->GetOwner() == GetOwner()->GetParent())
		{
			float2 new_pos = parent_used_canvas->GetPositionFromAnchorPoint(anchor_pos);

			new_pos += anchor_offset;

			SetPosition(new_pos);
		}
	}
}

float2 ComponentTransform::GetAnchorPos() const
{
	return anchor_pos;
}

ComponentCanvas * ComponentTransform::GetUsedCanvas() const
{
	return used_canvas_comp;
}

ComponentCanvas * ComponentTransform::GetParentUsedCanvas() const
{
	return parent_used_canvas;
}

ScriptingBridgeComponentTransform * ComponentTransform::GetScriptingBridge() const
{
	return scripting_bridge;
}

void ComponentTransform::UpdateWorldAndLocalValues()
{
	float3 pos = float3::zero;
	Quat rot = Quat::identity;
	float3 scal = float3::zero;

	local_transform.Decompose(pos, rot, scal);

	local_pos = float2(pos.x, pos.y);
	local_rotation = rot.ToEulerXYZ().z;
	local_scale = float2(scal.x, scal.y);

	pos = float3::zero;
	rot = Quat::identity;
	scal = float3::zero;

	world_transform.Decompose(pos, rot, scal);

	base_physics_body->SetPosition(float2(pos.x, pos.y));
	base_physics_body->SetRotationDegrees(rot.ToEulerXYZ().z * RADTODEG);
	world_scale = float2(scal.x, scal.y);
}

void ComponentTransform::UpdateLocalTransformFromValues()
{
	local_transform = float4x4::FromTRS(float3(local_pos.x, local_pos.y, 0),
		Quat::FromEulerXYZ(0, 0, local_rotation),
		float3(local_scale.x, local_scale.y, 1));

	RecalculateWorldTransform();
}

void ComponentTransform::UpdateWorldTransformFromValues()
{
	float2 world_pos = base_physics_body->GetPosition();
	float world_rotation = base_physics_body->GetRotationDegrees();

	world_transform = float4x4::FromTRS(float3(world_pos.x, world_pos.y, 0),
		Quat::FromEulerXYZ(0, 0, world_rotation * DEGTORAD),
		float3(world_scale.x, world_scale.y, 1));

	RecalculateLocalTransform();
}

void ComponentTransform::UpdatePhysicsMovement()
{
	if (used_physics_body_comp != nullptr)
	{
		float2 curr_pos = base_physics_body->GetPosition();

		if (last_pos.x != curr_pos.x || last_pos.y != curr_pos.y)
		{
			SetPosition(curr_pos);
		}

		float curr_rotation = base_physics_body->GetRotationDegrees();

		if (last_rotation != curr_rotation)
		{
			SetRotationDegrees(curr_rotation);
		}
	}
}

void ComponentTransform::FindParentUsedCanvas()
{
	bool had_parent_canvas = false;

	if (parent_used_canvas != nullptr)
		had_parent_canvas = true;

	parent_used_canvas = nullptr;

	GameObject* parent = GetOwner()->GetParent();

	if (parent != nullptr)
		parent_used_canvas = parent->transform->GetUsedCanvas();

	if(had_parent_canvas && parent_used_canvas == nullptr)
		App->guizmo->canvas_item_guizmo->StopEditing(this);

	if(!had_parent_canvas && parent_used_canvas != nullptr)
		App->guizmo->canvas_item_guizmo->StartEditing(this);
}

void ComponentTransform::RecalculateCanvasLayout()
{
	SetAnchorPos(anchor_pos);
}

void ComponentTransform::RecalculateWorldTransform()
{
	float4x4 parent_world_transform = float4x4::identity;

	if (owner->GetParent() != nullptr)
	{
		parent_world_transform = owner->GetParent()->transform->GetWorldTransform();
	}

	float4x4 check_world_trans = parent_world_transform * local_transform;

	if (!world_transform.Equals(check_world_trans))
	{
		world_transform = check_world_trans;

		UpdateLastValues();

		UpdateWorldAndLocalValues();
	}
}

void ComponentTransform::RecalculateLocalTransform()
{
	float4x4 parent_world_transform = float4x4::identity;

	if (owner->GetParent() != nullptr)
	{
		parent_world_transform = owner->GetParent()->transform->GetWorldTransform();
	}

	float4x4 check_local_trans = parent_world_transform.Inverted() * world_transform;

	if (!local_transform.Equals(check_local_trans))
	{
		local_transform = check_local_trans;

		UpdateLastValues();

		UpdateWorldAndLocalValues();
	}
}

void ComponentTransform::UpdateLastValues()
{
	float3 pos = float3::zero; 
	Quat rot = Quat::identity; 
	float3 scal = float3::zero;

	world_transform.Decompose(pos, rot, scal);

	last_pos = float2(pos.x, pos.y);
	last_rotation = rot.ToEulerXYZ().z * RADTODEG;
}

