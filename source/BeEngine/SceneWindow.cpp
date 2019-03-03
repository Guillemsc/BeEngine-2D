#include "SceneWindow.h"
#include "App.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "ModuleEvent.h"
#include "Event.h"
#include "ModuleGuizmo.h"
#include "ModulePhysics.h"

SceneWindow::SceneWindow()
{
}

SceneWindow::~SceneWindow()
{
}

void SceneWindow::Start()
{
	font = App->editor->GetLoadedFont("RobotoCondensed_16");
}

void SceneWindow::CleanUp()
{
}

void SceneWindow::DrawEditor()
{
	float2 window_size = GetWindowSize();
	float2 window_pos = GetWindowPos();

	float2 image_pos = float2(ImGui::GetCursorPos().x, ImGui::GetCursorPos().y);
	float2 image_size = float2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);

	scene_viewport_rect = Rect(window_pos.x + image_pos.x, window_pos.y + image_pos.y, image_size.x, image_size.y);

	if (last_size.x != image_size.x || last_size.y != image_size.y)
	{
		App->event->SendEvent(new EventSceneWindowResize(last_size, image_size));

		last_size = image_size;

		App->camera->GetEditorCamera()->SetViewportSize(image_size.x, image_size.y);
	}

	ImGui::PushFont(font);
	if (ImGui::BeginMenuBar())
	{
		bool guizmos_handlers = App->guizmo->GetRenderHandlers();
		if (ImGui::Checkbox("Handlers Guizmos", &guizmos_handlers))
		{
			App->guizmo->SetRenderHandlers(guizmos_handlers);
		}

		bool guizmos_physics = App->guizmo->GetRenderAllPhysicsGuizmos();
		if (ImGui::Checkbox("Physics Guizmos", &guizmos_physics))
		{
			App->guizmo->SetRenderAllPhysicsGuizmos(guizmos_physics);
		}

		ImGui::MenuItem("Main menu bar", NULL);
		ImGui::EndMenuBar();
	}
	ImGui::PopFont();
	
	ImGui::Image((void*)App->camera->GetEditorCamera()->GetTextId(), { image_size.x, image_size.y}, ImVec2(0, 1), ImVec2(1, 0));
}

ImGuiWindowFlags SceneWindow::GetWindowFlags()
{
	return ImGuiWindowFlags_MenuBar;
}

Rect SceneWindow::GetSceneRect() const
{
	return scene_viewport_rect;
}
