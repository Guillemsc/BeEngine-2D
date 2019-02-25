#include "ComponentPhysicsBody.h"
#include "App.h"
#include "ModulePhysics.h"
#include "PhysicsBody.h"

ComponentPhysicsBody::ComponentPhysicsBody() 
	: GameObjectComponent("Physics Body", ComponentType::COMPONENT_TYPE_PHYSICS_BODY, ComponentGroup::PHYSICS, true, true)
{
}

ComponentPhysicsBody::~ComponentPhysicsBody()
{
}

void ComponentPhysicsBody::EditorDraw()
{
}

void ComponentPhysicsBody::Start()
{
	physics_body = App->physics->CreatePhysicsBody();
	physics_body->SetComponentPhysicsBody(this);
}

void ComponentPhysicsBody::Update()
{
}

void ComponentPhysicsBody::CleanUp()
{
	App->physics->DestroyPhysicsBody(physics_body);
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
