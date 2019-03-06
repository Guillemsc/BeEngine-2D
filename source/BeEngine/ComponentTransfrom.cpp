#include "ComponentTransfrom.h"
#include "GameObject.h"
#include "imgui.h"
#include "ModuleJson.h"
#include "ScriptingBridgeComponentTransform.h"
#include "ModuleScripting.h"
#include "App.h"
#include "PhysicsBody.h"

ComponentTransform::ComponentTransform() 
	: GameObjectComponent("Transform", ComponentType::COMPONENT_TYPE_TRANSFORM, ComponentGroup::TRANSFORMATIONS, true, false)
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

	float2 world_position = GetPosition();

	if (ImGui::DragFloat2("Position", (float*)&position, 0.1f))
		SetLocalPosition(position);

	if (ImGui::DragFloat("Rotation", (float*)&rotation, 0.1f))
		SetLocalRotationDegrees(rotation);

	if (ImGui::DragFloat2("Scale", (float*)&scale, 0.1f))
		SetLocalScale(scale);

	ImGui::Checkbox("Keep scale ratio", &keep_scale_ratio);

	ImGui::Text("World Positions: ");

	std::string x_world = "x: " + std::to_string(world_position.x);
	std::string y_world = "y: " + std::to_string(world_position.y);

	ImGui::Text(x_world.c_str());
	ImGui::SameLine();
	ImGui::Text(y_world.c_str());

}

void ComponentTransform::Start()
{
	scripting_bridge = new ScriptingBridgeComponentTransform(this);
	App->scripting->AddScriptingBridgeObject(scripting_bridge);

	base_physics_body = App->physics->CreatePhysicsBody();
	base_physics_body->SetType(PhysicsBodyType::PHYSICS_BODY_KINEMATIC);
	base_physics_body->SetComponentTransform(this);

	local_transform = float4x4::identity;
	world_transform = float4x4::identity;

	UpdateLocalTransformFromValues();
}

void ComponentTransform::Update()
{
	UpdatePhysicsMovement();
}

void ComponentTransform::CleanUp()
{
	App->physics->DestroyPhysicsBody(base_physics_body);

	App->scripting->DestroyScriptingBridgeObject(scripting_bridge);
}

void ComponentTransform::OnSaveAbstraction(DataAbstraction & abs)
{
	abs.AddFloat2("position", GetPosition());
	abs.AddFloat2("scale", GetScale());
	abs.AddFloat("rotation", GetRotationDegrees());
	abs.AddBool("keep_ratio", keep_scale_ratio);
}

void ComponentTransform::OnLoadAbstraction(DataAbstraction & abs)
{
	keep_scale_ratio = abs.GetBool("keep_ratio");
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

void ComponentTransform::OnAddComponent(GameObjectComponent * new_component)
{
	if (new_component->GetType() == ComponentType::COMPONENT_TYPE_PHYSICS_BODY)
	{
		base_physics_body->SetType(PhysicsBodyType::PHYSICS_BODY_DYNAMIC);
		base_physics_body->ClearForces();
	}
}

void ComponentTransform::OnRemoveComponent(GameObjectComponent * deleted)
{
	if (deleted->GetType() == ComponentType::COMPONENT_TYPE_PHYSICS_BODY)
	{
		base_physics_body->SetType(PhysicsBodyType::PHYSICS_BODY_KINEMATIC);
		base_physics_body->ClearForces();
	}
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
	return local_pos;
}

float ComponentTransform::GetLocalRotationDegrees()
{
	return local_rotation * RADTODEG;
}

float2 ComponentTransform::GetLocalScale()
{
	return local_scale;
}

float2 ComponentTransform::GetPosition()
{
	return base_physics_body->GetPosition();
}

float ComponentTransform::GetRotationDegrees()
{
	return base_physics_body->GetRotationDegrees();
}

float2 ComponentTransform::GetScale()
{
	return world_scale;
}

float4x4 ComponentTransform::GetLocalTransform()
{
	return local_transform;
}

float4x4 ComponentTransform::GetWorldTransform()
{
	float4x4 parent_world_transform = float4x4::identity;

	if (owner->GetParent() != nullptr)
	{
		parent_world_transform = owner->GetParent()->transform->GetWorldTransform();
	}

	float4x4 test_trans = parent_world_transform * local_transform;

	if (!world_transform.Equals(test_trans))
	{
		UpdateWorldFromLocalTransform();
	}

	return world_transform;
}

ScriptingBridgeComponentTransform * ComponentTransform::GetScriptingBridge() const
{
	return scripting_bridge;
}

void ComponentTransform::UpdateLocalFromWorldTransform()
{
	float4x4 parent_world_transform = float4x4::identity;

	if (owner->GetParent() != nullptr)
	{
		parent_world_transform = owner->GetParent()->transform->GetWorldTransform();
	}

	local_transform = parent_world_transform.Inverted() * world_transform;

	if (keep_scale_ratio)
	{
		if (local_transform[0][0] != local_transform[1][1])
		{
			local_transform[1][1] = local_transform[0][0];
		}
	}

	UpdateWorldAndLocalValues();
}

void ComponentTransform::UpdateWorldFromLocalTransform()
{
	float4x4 parent_world_transform = float4x4::identity;

	if(owner->GetParent() != nullptr)
	{
		parent_world_transform = owner->GetParent()->transform->GetWorldTransform();
	}

	world_transform = parent_world_transform * local_transform;

	if (keep_scale_ratio)
	{
		if (world_transform[0][0] != world_transform[1][1])
		{
			world_transform[1][1] = world_transform[0][0];
		}
	}

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

	base_physics_body->SetPosition(float2(pos.x, pos.y));
	base_physics_body->SetRotationDegrees(rot.ToEulerXYZ().z * RADTODEG);
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
	float2 world_pos = base_physics_body->GetPosition();
	float world_rotation = base_physics_body->GetRotationDegrees();

	world_transform = float4x4::FromTRS(float3(world_pos.x, world_pos.y, 0),
		Quat::FromEulerXYZ(0, 0, world_rotation * DEGTORAD),
		float3(world_scale.x, world_scale.y, 1));

	last_pos = world_pos;
	last_rotation = world_rotation;

	UpdateLocalFromWorldTransform();
}

void ComponentTransform::UpdatePhysicsMovement()
{
	if (used_physics_body_comp != nullptr)
	{
		float2 curr_pos = base_physics_body->GetPosition();

		if (last_pos.x != curr_pos.x || last_pos.y != curr_pos.y)
		{
			SetPosition(base_physics_body->GetPosition());
		}

		float curr_rotation = base_physics_body->GetRotationDegrees();

		if (last_rotation != curr_rotation)
		{
			base_physics_body->SetRotationDegrees(curr_rotation);
		}
	}
}

