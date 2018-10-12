#include "GameWindow.h"
#include "App.h"
#include "imgui.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"

GameWindow::GameWindow()
{
}

GameWindow::~GameWindow()
{
}

void GameWindow::CleanUp()
{
}

void GameWindow::DrawEditor()
{
	float2 window_size = GetWindowSize();

	ImGui::Image((void*)App->camera->GetEditorCamera()->GetTextId(), { window_size.x, window_size.y }, ImVec2(0, 1), ImVec2(1, 0));
}
