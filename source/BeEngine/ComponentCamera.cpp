#include "ComponentCamera.h"
#include "App.h"
#include "ModuleCamera.h"
#include "imgui.h"
#include "GameObject.h"
#include "ComponentTransfrom.h"
#include "ModuleJson.h"
#include "LineRenderer.h"
#include "ModuleSceneRenderer.h"

ComponentCamera::ComponentCamera() : GameObjectComponent("Camera", ComponentType::COMPONENT_TYPE_CAMERA, ComponentGroup::CAMERA, true)
{
}

ComponentCamera::~ComponentCamera()
{
}

void ComponentCamera::EditorDraw()
{
	if (camera != nullptr)
	{
		Camera2D* curr_game_camera = App->camera->GetGameCamera();

		bool is_current = curr_game_camera == camera;

		if (is_current)
		{
			ImGui::Text("Is current game camera.");
		}
		else
		{
			if (ImGui::Button("Set as current game camera"))
			{
				App->camera->SetGameCamera(camera);
			}
		}

		float size = camera->GetSize();

		if (ImGui::DragFloat("Size", &size, 0.001f, 0.0001f, 999))
		{
			camera->SetSize(size);
		}
	}
}

void ComponentCamera::Start()
{
	camera = App->camera->CreateCamera();

	if (App->camera->GetGameCamera() == nullptr)
	{
		App->camera->SetGameCamera(camera);
	}
}

void ComponentCamera::Update()
{
	UpdateCameraPos();
}

void ComponentCamera::CleanUp()
{
	if (App->camera->GetGameCamera() == camera)
		App->camera->SetGameCamera(nullptr);

	App->camera->DestroyCamera(camera);
}

void ComponentCamera::OnSaveAbstraction(DataAbstraction & abs)
{	
	abs.AddFloat("size", camera->GetSize());
}

void ComponentCamera::OnLoadAbstraction(DataAbstraction & abs)
{	
	camera->SetSize(abs.GetFloat("size"));
}

void ComponentCamera::OnEvent(Event * ev)
{
}

void ComponentCamera::OnChildAdded(GameObject * child)
{
}

void ComponentCamera::OnChildRemoved(GameObject * child)
{
}

void ComponentCamera::OnParentChanged(GameObject * new_parent)
{
}

void ComponentCamera::RenderGuizmos(float relative_size)
{
	std::vector<float2> points = camera->GetCorners();

	float3 colour = float3(63.0f / 255.0f, 211.0f / 255.0f, 115.0f / 255.0f);

	float thickness = 1.5f * relative_size;

	App->scene_renderer->line_renderer->SetZPos(App->scene_renderer->layer_space_guizmos.GetLayerValue(99));

	App->scene_renderer->line_renderer->DrawLine(points[0], points[1], colour, 1, thickness);
	App->scene_renderer->line_renderer->DrawLine(points[1], points[2], colour, 1, thickness);
	App->scene_renderer->line_renderer->DrawLine(points[2], points[3], colour, 1, thickness);
	App->scene_renderer->line_renderer->DrawLine(points[3], points[0], colour, 1, thickness);
}

Camera2D * ComponentCamera::GetCamera()
{
	return camera;
}

void ComponentCamera::UpdateCameraPos()
{
	camera->SetPosition(owner->transform->GetPosition());
}
