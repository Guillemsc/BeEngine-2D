#include "ComponentPhysicsBody.h"
#include "App.h"
#include "ModulePhysics.h"

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
}

void ComponentPhysicsBody::Update()
{
}

void ComponentPhysicsBody::CleanUp()
{
	App->physics->DestroyPhysicsBody(physics_body);
}
