#include "ComponentPolygonCollider.h"
#include "PhysicsShape.h"
#include "ModulePhysics.h"
#include "App.h"
#include "PhysicsShapePolygon.h"
#include "GameObject.h"
#include "ComponentTransfrom.h"
#include "PhysicsBody.h"
#include "imgui.h"
#include "ModuleJson.h"
#include "ModuleGuizmo.h"
#include "PhysicsPolygonGuizmo.h"
#include "ModuleGameObject.h"
#include "ModuleSceneRenderer.h"
#include "LineRenderer.h"


ComponentPolygonCollider::ComponentPolygonCollider() 
	: GameObjectComponent("Polygon Collider", ComponentType::COMPONENT_TYPE_POLYGON_COLLIDER, ComponentGroup::PHYSICS)
{
}

ComponentPolygonCollider::~ComponentPolygonCollider()
{
}

void ComponentPolygonCollider::EditorDraw()
{
	bool is_sensor = physics_shape->GetIsSensor();

	if (App->guizmo->physics_polygon_guizmo->GetEditingComponent() != this)
	{
		if (ImGui::Button("Edit"))
		{
			App->gameobject->AddGameObjectAsOnlySelected(GetOwner());
			App->guizmo->physics_polygon_guizmo->StartEditing(this);
		}
	}
	else
	{
		if (ImGui::Button("Stop Editing"))
		{
			std::vector<float2> vertices = App->guizmo->physics_polygon_guizmo->FinishEditing(this);
			physics_shape->SetVertices(vertices);
		}
	}

	if (ImGui::Checkbox("Is sensor", &is_sensor))
	{
		physics_shape->SetIsSensor(is_sensor);
	}
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
	App->guizmo->physics_polygon_guizmo->StopEditing(this);

	GetOwner()->transform->base_physics_body->RemoveShape(physics_shape);
	App->physics->DestroyPhysicsShape(physics_shape);
}

void ComponentPolygonCollider::OnSaveAbstraction(DataAbstraction & abs)
{
	abs.AddBool("is_sensor", physics_shape->GetIsSensor());
	abs.AddFloat2Vector("vertices", physics_shape->GetVertices());
}

void ComponentPolygonCollider::OnLoadAbstraction(DataAbstraction & abs)
{
	physics_shape->SetIsSensor(abs.GetBool("is_sensor"));
	physics_shape->SetVertices(abs.GetFloat2Vector("vertices"));
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

void ComponentPolygonCollider::OnOwnerSelected()
{
}

void ComponentPolygonCollider::OnOwnerDeSelected()
{
	App->guizmo->physics_polygon_guizmo->StopEditing(this);
}

void ComponentPolygonCollider::RenderGuizmos(float relative_size)
{
	if (App->guizmo->physics_polygon_guizmo->GetEditingComponent() != this)
	{
		std::vector<float2> vertices = physics_shape->GetVertices();

		PhysicsBody* body = physics_shape->GetAttachedBody();

		App->scene_renderer->line_renderer->SetZPos(App->scene_renderer->layer_space_guizmos.GetLayerValue(100));

		float thickness = 2 * relative_size;

		int counter = 0;
		for (std::vector<float2>::iterator it = vertices.begin(); it != vertices.end(); ++it, ++counter)
		{
			float2 curr_pos = body->LocalPointToWorldPoint((*it));

			float2 next_pos = float2::zero;

			if (counter >= vertices.size() - 1)
				next_pos = body->LocalPointToWorldPoint(vertices[0]);
			else
				next_pos = body->LocalPointToWorldPoint(vertices[counter + 1]);

			App->scene_renderer->line_renderer->DrawLine(curr_pos, next_pos, float3(0, 1, 0), 1, thickness);
		}
	}
}

PhysicsShapePolygon * ComponentPolygonCollider::GetPhysicsShapePolygon() const
{
	return physics_shape;
}


