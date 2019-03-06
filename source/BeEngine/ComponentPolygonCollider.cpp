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
		if (ImGui::Button("Save Editing"))
		{
			std::vector<float2> vertices = App->guizmo->physics_polygon_guizmo->FinishEditing(this);
			physics_shape->SetVertices(vertices);
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel Editing"))
		{
			App->guizmo->physics_polygon_guizmo->StopEditing(this);
		}
	}

	if (physics_shape->GetShapeTooSmall())
	{
		ImGui::Separator();
		ImGui::Text("The collider did not create any collision shapes as they");
		ImGui::Text("failed verification. This could be because they were deemed");
		ImGui::Text("too small or the vertices were too close.");
		ImGui::Separator();
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
		float thickness = 2 * relative_size;

		float3 colour = float3(0, 1, 0);

		if (physics_shape->GetShapeTooSmall())
			colour = float3(1, 0, 0);

		PhysicsBody* body = physics_shape->GetAttachedBody();

		std::vector<std::vector<float2>> triangles = physics_shape->GetTriangles();

		for (std::vector<std::vector<float2>>::iterator it = triangles.begin(); it != triangles.end(); ++it)
		{
			std::vector<float2> triangle = (*it);

			int counter = 0;
			for (std::vector<float2>::iterator tr = triangle.begin(); tr != triangle.end(); ++tr, ++counter)
			{
				float2 curr_pos = body->LocalPointToWorldPoint((*tr));

				float2 next_pos = float2::zero;

				if (counter >= triangle.size() - 1)
					next_pos = body->LocalPointToWorldPoint(triangle[0]);
				else
					next_pos = body->LocalPointToWorldPoint(triangle[counter + 1]);

				App->scene_renderer->line_renderer->DrawLine(curr_pos, next_pos, colour, 1, thickness);
			}
		}
	}
}

PhysicsShapePolygon * ComponentPolygonCollider::GetPhysicsShapePolygon() const
{
	return physics_shape;
}


