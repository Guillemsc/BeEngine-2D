#include "ComponentTransfrom.h"
#include "GameObject.h"
#include "imgui.h"

ComponentTransform::ComponentTransform() : GameObjectComponent("Transform", ComponentType::TRANSFORM, ComponentGroup::TRANSFORMATIONS, true, false)
{
}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::EditorDraw()
{
	float2 position = GetPosition();
	float rotation = GetRotationAngles();
	float2 scale = GetScale();

	if (ImGui::DragFloat2("Position", (float*)&position, 0.1f))
		SetPosition(position);

	if (ImGui::DragFloat("Rotation", (float*)&rotation, 0.1f))
		SetRotationAngles(rotation);

	if (ImGui::DragFloat2("Scale", (float*)&scale, 0.1f, 0.0f))
		SetScale(scale);
}

void ComponentTransform::Start()
{
	local_transform = float4x4::identity;

	UpdateLocalTransform();
}

void ComponentTransform::CleanUp()
{
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

void ComponentTransform::SetPosition(const float2 & pos)
{
	if (pos.x != local_pos.x || pos.y != local_pos.y)
	{
		local_pos = pos;

		UpdateLocalTransform();
	}
}

void ComponentTransform::SetRotationAngles(float rotation)
{
	rotation *= DEGTORAD;

	if (rotation != local_rotation)
	{
		local_rotation = rotation;

		UpdateLocalTransform();
	}
}

void ComponentTransform::SetScale(const float2 & scale)
{
	if (scale.x != local_scale.x || scale.y != local_scale.y)
	{
		local_scale = scale;

		if (local_scale.y < 0)
			local_scale.y = 0;

		if (local_scale.y < 0)
			local_scale.y = 0;

		UpdateLocalTransform();
	}
}

float2 ComponentTransform::GetPosition() const
{
	return local_pos;
}

float ComponentTransform::GetRotationAngles() const
{
	return local_rotation * RADTODEG;
}

float2 ComponentTransform::GetScale() const
{
	return local_scale;
}

void ComponentTransform::SetLocalTransform(const float4x4 & local)
{
	float3 pos;
	Quat rot;
	float3 scal;
	local.Decompose(pos, rot, scal);

	local_pos = float2(pos.x, pos.y);
	local_rotation = rot.ToEulerXYZ().z;
	local_scale = float2(scal.x, scal.y);

	UpdateLocalTransform();
}

float4x4 ComponentTransform::GetLocalTransform() const
{
	return local_transform;
}

float4x4 ComponentTransform::GetGlobalTransform() const
{
	return global_transform;
}

void ComponentTransform::UpdateLocalTransform()
{
	local_transform = float4x4::FromTRS(float3(local_pos.x, local_pos.y, 0), 
		Quat::FromEulerXYZ(0, 0, local_rotation), 
		float3(local_scale.x, local_scale.y, 1));

	UpdateChildsTreeGlobalTransform();
}

void ComponentTransform::UpdateChildsTreeGlobalTransform()
{
	if (owner->GetParent() != nullptr)
		global_transform = owner->GetParent()->transform->GetGlobalTransform() * local_transform;
	else
		global_transform = local_transform;

	std::vector<GameObject*> childs = owner->GetChilds();

	for (std::vector<GameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
	{
		(*it)->transform->UpdateChildsTreeGlobalTransform();
	}
}
