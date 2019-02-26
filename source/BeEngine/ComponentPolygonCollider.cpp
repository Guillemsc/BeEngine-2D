#include "ComponentPolygonCollider.h"
#include "PhysicsShape.h"

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
}

void ComponentPolygonCollider::Update()
{
}

void ComponentPolygonCollider::CleanUp()
{
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

