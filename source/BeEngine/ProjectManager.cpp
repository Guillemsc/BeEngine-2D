#include "ProjectManager.h"
#include "App.h"
#include "ModuleWindow.h"

ProjectManager::ProjectManager()
{
}

ProjectManager::~ProjectManager()
{
}

void ProjectManager::Start()
{
	big_font = App->editor->GetLoadedFont("RobotoMedium_60");
	medium_font = App->editor->GetLoadedFont("RobotoMedium_30");
	small_font = App->editor->GetLoadedFont("RobotoMedium_18");
}

void ProjectManager::CleanUp()
{
}

void ProjectManager::DrawEditor()
{
	int margins = 60;

	float2 window_size = App->window->GetWindowSize();

	float2 docking_pos = float2(0, 0);
	float2 docking_size = float2(window_size.x, window_size.y );

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus;

	bool opened = true;

	ImGui::PushFont(big_font);

	ImGui::SetNextWindowPos(ImVec2(docking_pos.x, docking_pos.y));
	ImGui::SetNextWindowSize(ImVec2(docking_size.x, docking_size.y));
	if (ImGui::Begin("ProjectManager", &opened, flags))
	{
		ImGui::SetCursorScreenPos(ImVec2(margins, 40));

		ImGui::Text("BeEngine - Project Manager");

		ImGui::PopFont();

		ImGui::PushFont(medium_font);

		ImGui::SetCursorScreenPos(ImVec2(margins, 120));

		if (ImGui::Button("New Project"))
		{

		}

		ImGui::SameLine();

		if (ImGui::Button("Open Project"))
		{

		}

		ImGui::SetCursorScreenPos(ImVec2(margins, 175));
		ImGui::Separator();

		ImGui::BeginGroup();

		ImGui::SetCursorPosX(margins + (20));

		ImGui::BeginChild("Child", ImVec2(window_size.x - (margins * 2) - (20*2), window_size.y - 190), false);

		ImGui::PopFont();

		for (int i = 0; i < 15; ++i)
		{
			ImGui::PushFont(medium_font);

			ImGui::PushID(i);

			ImGui::SetCursorPosY(20 + (i * 100));

			std::string proj_name = "Project" + std::to_string(i);

			if (ImGui::Button(proj_name.c_str()))
			{
				App->editor->SetEditorState(EditorState::MAIN_ENGINE);
			}

			ImGui::PopFont();

			ImGui::PushFont(small_font);

			ImGui::Text("Path: C:\\Users\\Guillem\\Documents\\GitHub\\BeEngine-2D\\source\\Debug\\fonts");

			ImGui::PopID();

			ImGui::SetCursorPosY(90 + (i * 100));

			ImGui::Separator();

			ImGui::PopFont();
		}

		ImGui::EndChild();

		ImGui::EndGroup();
	}

	ImGui::End();
}
