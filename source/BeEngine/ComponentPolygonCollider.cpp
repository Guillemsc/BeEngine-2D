#include "ComponentPolygonCollider.h"
#include "PhysicsShape.h"
#include "ModulePhysics.h"
#include "App.h"
#include "PhysicsShapePolygon.h"
#include "GameObject.h"
#include "ComponentTransfrom.h"
#include "PhysicsBody.h"

ComponentPolygonCollider::ComponentPolygonCollider() 
	: GameObjectComponent("Polygon Collider", ComponentType::COMPONENT_TYPE_POLYGON_COLLIDER, ComponentGroup::PHYSICS)
{
}

ComponentPolygonCollider::~ComponentPolygonCollider()
{
}

void ComponentPolygonCollider::EditorDraw()
{
}

void ComponentPolygonCollider::Start()
{
	physics_shape = (PhysicsShapePolygon*)App->physics->CreatePhysicsShape(PhysicsShapeType::PHYSICS_SHAPE_POLYGON);
	GetOwner()->transform->base_physics_body->AddShape(physics_shape);
}

void ComponentPolygonCollider::Update()
{
}

void ComponentPolygonCollider::CleanUp()
{
	GetOwner()->transform->base_physics_body->RemoveShape(physics_shape);
	App->physics->DestroyPhysicsShape(physics_shape);
}

void ComponentPolygonCollider::OnSaveAbstraction(DataAbstraction & abs)
{
}

void ComponentPolygonCollider::OnLoadAbstraction(DataAbstraction & abs)
{
}

void ComponentPolygonCollider::OnEvent(Event * ev)
{
}

void ComponentPolygonCollider::OnChildAdded(GameObject * child)
{
}

void ComponentPolygonCollider::OnChildRemoved(GameObject * child)
{
}

void ComponentPolygonCollider::OnParentChanged(GameObject * new_parent)
{
}

void ComponentPolygonCollider::DrawGuizmos()
{
	physics_shape->GuizmoDraw();
}

