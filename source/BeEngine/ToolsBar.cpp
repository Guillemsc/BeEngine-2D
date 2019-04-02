#include "ToolsBar.h"
#include "App.h"
#include "imgui.h"
#include "ModuleWindow.h"
#include "ModuleState.h"
#include "ModuleFileSystem.h"
#include "ModuleBuild.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

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

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus
		| ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoScrollbar;

	ImGui::SetNextWindowPos(ImVec2(tools_bar_pos.x, tools_bar_pos.y));
	ImGui::SetNextWindowSize(ImVec2(tools_bar_size.x, tools_bar_size.y));

	bool opened = true;
	if (ImGui::Begin("Tools Bar", &opened, flags))
	{
		if (App->state->GetEditorUpdateState() == EditorUpdateState::EDITOR_UPDATE_STATE_IDLE)
		{
			if (ImGui::Button("Build"))
			{
				bool cancelled = false;
				std::string folder = App->file_system->SelectFolderDialog(cancelled);

				if (!cancelled)
				{
					std::vector<std::string> errors;
					App->build->GenerateBuild(folder, errors);
				}
			}

			ImGui::SameLine();

			if(ImGui::Button("Play"))
			{
				App->state->SetEditorUpdateState(EditorUpdateState::EDITOR_UPDATE_STATE_PLAY);
			}
		}
		else
		{
			if (ImGui::Button("Stop"))
			{
				App->state->SetEditorUpdateState(EditorUpdateState::EDITOR_UPDATE_STATE_IDLE);
			}

			ImGui::SameLine();

			if (ImGui::Button("Pause"))
			{
				App->state->SetEditorUpdateState(EditorUpdateState::EDITOR_UPDATE_STATE_PAUSED);
			}
		}
	}

	ImGui::End();
}
