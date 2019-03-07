#include "DockingSpace.h"
#include "App.h"
#include "ModuleWindow.h"
#include "ModuleEditor.h"
#include "imgui_docking.h"
#include "imgui.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

DockingSpace::DockingSpace(float2 _margins_left_up, float2 _margins_right_down)
{
	margins_left_up = _margins_left_up;
	margins_right_down = _margins_right_down;
}

DockingSpace::~DockingSpace()
{
}

void DockingSpace::Start()
{
	font = App->editor->GetLoadedFont("RobotoBold_17");
}

void DockingSpace::CleanUp()
{
}

void DockingSpace::DrawEditor()
{
	float2 window_size = App->window->GetWindowSize();

	float2 docking_pos = float2(margins_left_up.x - 2, margins_left_up.y - 5);
	float2 docking_size = float2(window_size.x - margins_right_down.x + 3 - margins_left_up.x,
		window_size.y - margins_right_down.y + 5 - margins_left_up.y);

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus;


	ImGui::PushFont(font);

	ImGui::RootDock(ImVec2(docking_pos.x, docking_pos.y), ImVec2(docking_size.x, docking_size.y));

	ImGui::PopFont();
}
