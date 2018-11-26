#include "BottomBar.h"
#include "App.h"
#include "ModuleWindow.h"
#include "ConsoleWindow.h"

BottomBar::BottomBar(float _height)
{
	height = _height;
}

BottomBar::~BottomBar()
{
}

void BottomBar::Start()
{
}

void BottomBar::CleanUp()
{
}

void BottomBar::DrawEditor()
{
	float2 window_size = App->window->GetWindowSize();

	float2 tools_bar_pos = float2(0, window_size.y - height - 3);
	float2 tools_bar_size = float2(window_size.x, height);

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus
		| ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_HorizontalScrollbar;

	ImGui::SetNextWindowPos(ImVec2(tools_bar_pos.x, tools_bar_pos.y));
	ImGui::SetNextWindowSize(ImVec2(tools_bar_size.x, tools_bar_size.y));

	if (ImGui::Begin("Bottom Bar", &opened, flags))
	{
		ConsoleLogLine last_log = App->editor->console_window->GetLastLog();

		App->editor->console_window->DrawLog(last_log);
	}

	ImGui::End();
}
