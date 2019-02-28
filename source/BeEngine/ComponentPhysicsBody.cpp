#include "ComponentPhysicsBody.h"
#include "App.h"
#include "ModulePhysics.h"
#include "PhysicsBody.h"
#include "GameObject.h"
#include "ComponentTransfrom.h"
#include "imgui.h"

ComponentPhysicsBody::ComponentPhysicsBody() 
	: GameObjectComponent("Physics Body", ComponentType::COMPONENT_TYPE_PHYSICS_BODY, ComponentGroup::PHYSICS, true, true)
{
}

ComponentPhysicsBody::~ComponentPhysicsBody()
{
}

void ComponentPhysicsBody::EditorDraw()
{
	PhysicsBody* pb = GetOwner()->transform->base_physics_body;

	int curr_body_type = pb->GetType();

	const char* body_types = "Kinematic\0Dynamic\0Static\0";

	if (ImGui::Combo("Body type", &curr_body_type, body_types))
	{
		pb->SetType(static_cast<PhysicsBodyType>(curr_body_type));
	}

	float gravity_scale = pb->GetGravityScale();

	if (ImGui::DragFloat("Gravity scale", &gravity_scale, 0.1f))
	{
		pb->SetGravityScale(gravity_scale);
	}

	bool fixed_rotaton = pb->GetFixedRotation();

	if (ImGui::Checkbox("Fix rotation", &fixed_rotaton))
	{
		pb->SetFixedRotation(fixed_rotaton);
	}
}

void ComponentPhysicsBody::Start()
{
	GetOwner()->transform->used_physics_body_comp = this;
	GetOwner()->transform->base_physics_body->SetType(PhysicsBodyType::PHYSICS_BODY_DYNAMIC);
}

void ComponentPhysicsBody::Update()
{
}

void ComponentPhysicsBody::CleanUp()
{
	GetOwner()->transform->used_physics_body_comp == nullptr;
}

void ComponentPhysicsBody::OnSaveAbstraction(DataAbstraction & abs)
{
}

void ComponentPhysicsBody::OnLoadAbstraction(DataAbstraction & abs)
{
}

void ComponentPhysicsBody::OnEvent(Event * ev)
{
}

void ComponentPhysicsBody::OnChildAdded(GameObject * child)
{
}

void ComponentPhysicsBody::OnChildRemoved(GameObject * child)
{
}

void ComponentPhysicsBody::OnParentChanged(GameObject * new_parent)
{
}
