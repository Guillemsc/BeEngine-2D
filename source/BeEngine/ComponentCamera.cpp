#include "ComponentCamera.h"
#include "App.h"
#include "ModuleCamera.h"
#include "imgui.h"
#include "GameObject.h"
#include "ComponentTransfrom.h"

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

Camera2D * ComponentCamera::GetCamera()
{
	return camera;
}

void ComponentCamera::UpdateCameraPos()
{
	if (camera != nullptr)
	{
		camera->SetPosition(owner->transform->GetPosition());
	}
}
