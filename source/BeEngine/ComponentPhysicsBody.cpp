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

}

void ComponentPhysicsBody::Update()
{
}

void ComponentPhysicsBody::CleanUp()
{
	
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
