#include "ToolsBar.h"
#include "App.h"
#include "imgui.h"
#include "ModuleWindow.h"

ToolsBar::ToolsBar(float2 _margins_left_up)
{
	margins_left_up = _margins_left_up;
}

ToolsBar::~ToolsBar()
{
}

void ToolsBar::CleanUp()
{
}

void ToolsBar::DrawEditor()
{
	float2 window_size = App->window->GetWindowSize();

	float2 tools_bar_pos = float2(margins_left_up.x, margins_left_up.y);
	float2 tools_bar_size = float2(window_size.x, 39);

	bool opened = true;

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus
		| ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoScrollbar;

	ImGui::SetNextWindowPos(ImVec2(tools_bar_pos.x, tools_bar_pos.y));
	ImGui::SetNextWindowSize(ImVec2(tools_bar_size.x, tools_bar_size.y));

	ImGui::Begin("Tools Bar", &opened, flags);

	ImGui::Button("Press to die");

	ImGui::End();
}
