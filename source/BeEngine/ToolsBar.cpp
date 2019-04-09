#include "ToolsBar.h"
#include "App.h"
#include "imgui.h"
#include "ModuleWindow.h"
#include "ModuleState.h"
#include "ModuleFileSystem.h"
#include "ModuleBuild.h"
#include "BuildWindow.h"
#include "ModuleTexture.h"
#include "ModuleScripting.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

ToolsBar::ToolsBar(float2 _margins_left_up)
{
	margins_left_up = _margins_left_up;
}

ToolsBar::~ToolsBar()
{
}

void ToolsBar::Start()
{
	std::string play_logo_path = std::string("images\\Play.png");
	play_texture = App->texture->LoadTexture(play_logo_path);

	std::string pause_logo_path = std::string("images\\Pause.png");
	pause_texture = App->texture->LoadTexture(pause_logo_path);
}

void ToolsBar::CleanUp()
{
}

void ToolsBar::DrawEditor()
{
	float2 window_size = App->window->GetWindowSize();

	float2 tools_bar_pos = float2(margins_left_up.x, margins_left_up.y);
	float2 tools_bar_size = float2(window_size.x, 39);
	float tools_bar_half_size_x = tools_bar_size.x * 0.5f;

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
			ImGui::SetCursorPosX(tools_bar_half_size_x - 50);
			float2 play_size = play_texture->GetSize();
			if (ImGui::ImageButton((ImTextureID)play_texture->GetId(), ImVec2(play_size.x, play_size.y)))
			{
				App->state->SetEditorUpdateState(EditorUpdateState::EDITOR_UPDATE_STATE_PLAY);
			}
		}
		else
		{
			ImGui::SetCursorPosX(tools_bar_half_size_x - 50);
			float2 pause_size = pause_texture->GetSize();
			if (ImGui::ImageButton((ImTextureID)pause_texture->GetId(), ImVec2(pause_size.x, pause_size.y)))
			{
				App->state->SetEditorUpdateState(EditorUpdateState::EDITOR_UPDATE_STATE_IDLE);
			}
		}

		std::string curr_state = "";

		if (App->scripting->GetNeedsToCompileScripts())
		{
			curr_state = "Waiting for scripts to compile";
		}
		else if (!App->scripting->GetScriptsCompile())
		{
			curr_state = "Code does not compile";
		}
		else if (App->state->GetEditorUpdateState() == EditorUpdateState::EDITOR_UPDATE_STATE_PLAY)
		{
			curr_state = "Engine is playig now";
		}

		ImGui::SameLine();
		ImGui::SetCursorPos(ImVec2(tools_bar_half_size_x - 15, 13));
		ImGui::Text(curr_state.c_str());
	}

	ImGui::End();
}
