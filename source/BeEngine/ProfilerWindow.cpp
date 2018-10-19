#include "ProfilerWindow.h"
#include "App.h"
#include "Profiler.h"

ProfilerWindow::ProfilerWindow()
{
}

ProfilerWindow::~ProfilerWindow()
{
}

void ProfilerWindow::Start()
{
}

void ProfilerWindow::CleanUp()
{
}

void ProfilerWindow::DrawEditor()
{
	int draw_counter = 0;

	ImGui::Text("Starting Variables:");

	std::list<Profile*> start_profiles = App->profiler->GetStartProfilesList();

	for (std::list<Profile*>::iterator it = start_profiles.begin(); it != start_profiles.end(); ++it)
	{
		DrawProfileRecursive((*it), draw_counter, false);
	}

	ImGui::Separator();

	ImGui::Text("Updating Variables:");

	std::list<Profile*> update_profiles = App->profiler->GetUpdateProfilesList();

	for (std::list<Profile*>::iterator it = update_profiles.begin(); it != update_profiles.end(); ++it)
	{
		DrawProfileRecursive((*it), draw_counter, true);
	}
}

ImGuiWindowFlags ProfilerWindow::GetWindowFlags()
{
	return 0;
}

void ProfilerWindow::DrawProfileRecursive(Profile * prof, int& draw_counter, bool profiles_update)
{
	if (prof != nullptr)
	{
		uint flags = ImGuiTreeNodeFlags_OpenOnArrow;

		if (prof->GetChildProfilesCount() == 0)
			flags |= ImGuiTreeNodeFlags_Leaf;

		ImGui::PushID(draw_counter);

		float frame_data = 0.0f;
		if (!profiles_update)
			frame_data = prof->GetLastFrameMs();
		else
			frame_data = prof->GetAverageMs();

		bool opened = ImGui::TreeNodeEx(prof->GetName(), flags);

		ImGui::SameLine();

		std::string ms_text = std::string(": ") + std::to_string(frame_data) + " ms";

		ImGui::Text(ms_text.c_str());

		ImGui::PopID();

		++draw_counter;

		if (opened)
		{
			std::list<Profile*> childs = prof->GetChildProfiles();

			for (std::list<Profile*>::iterator it = childs.begin(); it != childs.end(); ++it)
			{
				DrawProfileRecursive(*it, draw_counter, profiles_update);
			}

			ImGui::TreePop();
		}
	}
}
