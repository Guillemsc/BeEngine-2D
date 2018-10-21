#include "SceneWindow.h"
#include "App.h"
#include "ModuleWindow.h"
#include "ModuleCamera.h"
#include "ModuleEvent.h"
#include "Event.h"

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

	if (last_size.x != window_size.x || last_size.y != window_size.y)
	{
		App->event->SendEvent(new EventSceneWindowResize(last_size, window_size));

		last_size = window_size;

		App->camera->GetEditorCamera()->SetViewportSize(window_size.x, window_size.y);
	}

	ImGui::PushFont(font);
	if (ImGui::BeginMenuBar())
	{
		bool as = true;
		ImGui::Checkbox("Wireframe", &as);
		ImGui::MenuItem("Main menu bar", NULL);
		ImGui::EndMenuBar();
	}
	ImGui::PopFont();
	

	ImGui::Image((void*)App->camera->GetEditorCamera()->GetTextId(), { window_size.x, window_size.y - 60}, ImVec2(0, 1), ImVec2(1, 0));
}

ImGuiWindowFlags SceneWindow::GetWindowFlags()
{
	return ImGuiWindowFlags_MenuBar;
}
