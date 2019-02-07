#include "ComponentTransfrom.h"
#include "GameObject.h"
#include "imgui.h"
#include "ModuleJson.h"

ComponentTransform::ComponentTransform() : GameObjectComponent("Transform", ComponentType::COMPONENT_TYPE_TRANSFORM, ComponentGroup::TRANSFORMATIONS, true, false)
{
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::EditorDraw()
{
	float2 position = GetLocalPosition();
	float rotation = GetLocalRotationDegrees();
	float2 scale = GetLocalScale();

	if (ImGui::DragFloat2("Position", (float*)&position, 0.1f))
		SetLocalPosition(position);

	if (ImGui::DragFloat("Rotation", (float*)&rotation, 0.1f))
		SetLocalRotationDegrees(rotation);

	if (ImGui::DragFloat2("Scale", (float*)&scale, 0.1f))
		SetLocalScale(scale);
}

void ComponentTransform::Start()
{
	local_transform = float4x4::identity;
	world_transform = float4x4::identity;

	UpdateLocalTransformFromValues();
}

void ComponentTransform::CleanUp()
{
}

void ComponentTransform::OnSaveAbstraction(DataAbstraction & abs)
{
	abs.AddFloat2("position", GetPosition());
	abs.AddFloat2("scale", GetScale());
	abs.AddFloat("rotation", GetRotationDegrees());
}

void ComponentTransform::OnLoadAbstraction(DataAbstraction & abs)
{
	SetPosition(abs.GetFloat2("position"));
	SetScale(abs.GetFloat2("scale"));
	SetRotationDegrees(abs.GetFloat("rotation"));
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

void ComponentTransform::SetLocalTransform(const float4x4 & local)
{
	local_transform = local;

	UpdateWorldFromLocalTransform();
}

void ComponentTransform::SetWorldTransform(const float4x4 & world)
{
	world_transform = world;

	UpdateLocalFromWorldTransform();
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
	if (scale.x != local_scale.x || scale.y != local_scale.y)
	{
		local_scale = scale;

		if (local_scale.x <= 0)
			local_scale.x = 0.01f;

		if (local_scale.y <= 0)
			local_scale.y = 0.01f;

		UpdateLocalTransformFromValues();
	}
}

void ComponentTransform::SetPosition(const float2 & pos)
{
	if (pos.x != world_pos.x || pos.y != world_pos.y)
	{
		world_pos = pos;

		UpdateWorldTransformFromValues();
	}
}

void ComponentTransform::SetRotationDegrees(float rotation)
{
	rotation *= DEGTORAD;

	if (rotation != world_rotation)
	{
		world_rotation = rotation;

		UpdateWorldTransformFromValues();
	}
}

void ComponentTransform::SetScale(const float2 & scale)
{
	if (scale.x != world_scale.x || scale.y != world_scale.y)
	{
		world_scale = scale;

		if (world_scale.x <= 0)
			world_scale.x = 0.01f;

		if (world_scale.y <= 0)
			world_scale.y = 0.01f;

		UpdateWorldTransformFromValues();
	}
}

float2 ComponentTransform::GetLocalPosition() const
{
	return local_pos;
}

float ComponentTransform::GetLocalRotationDegrees() const
{
	return local_rotation * RADTODEG;
}

float2 ComponentTransform::GetLocalScale() const
{
	return local_scale;
}

float2 ComponentTransform::GetPosition() const
{
	return world_pos;
}

float ComponentTransform::GetRotationDegrees() const
{
	return world_rotation * RADTODEG;
}

float2 ComponentTransform::GetScale() const
{
	return world_scale;
}

float4x4 ComponentTransform::GetLocalTransform() const
{
	return local_transform;
}

float4x4 ComponentTransform::GetWorldTransform() const
{
	float4x4 ret = float4x4::identity;

	if (owner->GetParent() != nullptr)
	{
		ret = owner->GetParent()->transform->GetWorldTransform();
	}

	ret = ret * local_transform;

	return ret;
}

void ComponentTransform::UpdateLocalFromWorldTransform()
{
	float4x4 parent_world_transform = float4x4::identity;

	if (owner->GetParent() != nullptr)
	{
		parent_world_transform = owner->GetParent()->transform->world_transform;
	}

	local_transform = parent_world_transform.Inverted() * world_transform;

	UpdateWorldAndLocalValues();
}

void ComponentTransform::UpdateWorldFromLocalTransform()
{
	float4x4 parent_world_transform = float4x4::identity;

	if(owner->GetParent() != nullptr)
	{
		parent_world_transform = owner->GetParent()->transform->world_transform;
	}

	world_transform = parent_world_transform * local_transform;

	UpdateWorldAndLocalValues();
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

	world_pos = float2(pos.x, pos.y);
	world_rotation = rot.ToEulerXYZ().z;
	world_scale = float2(scal.x, scal.y);
}

void ComponentTransform::UpdateLocalTransformFromValues()
{
	local_transform = float4x4::FromTRS(float3(local_pos.x, local_pos.y, 0),
		Quat::FromEulerXYZ(0, 0, local_rotation),
		float3(local_scale.x, local_scale.y, 1));

	UpdateWorldFromLocalTransform();
}

void ComponentTransform::UpdateWorldTransformFromValues()
{
	world_transform = float4x4::FromTRS(float3(world_pos.x, world_pos.y, 0),
		Quat::FromEulerXYZ(0, 0, world_rotation),
		float3(world_scale.x, world_scale.y, 1));

	UpdateLocalFromWorldTransform();
}

