#include "ProgressWindow.h"
#include "imgui.h"
#include "App.h"
#include "ModuleWindow.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

ProgressWindow::ProgressWindow()
{
}

ProgressWindow::~ProgressWindow()
{
}

void ProgressWindow::Start()
{
}

void ProgressWindow::CleanUp()
{
}

void ProgressWindow::DrawEditor()
{
	if (processes.size() > 0)
	{
		uint flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize
			| ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoSavedSettings
			| ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoTitleBar;

		float2 window_size = App->window->GetWindowSize();
		ImGui::SetNextWindowPos(ImVec2(window_size.x - last_window_size.x - 25, window_size.y - last_window_size.y - 10));

		if (ImGui::Begin("ProgressWindow", nullptr, flags))
		{
			for (std::map<std::string, float>::iterator it = processes.begin(); it != processes.end(); ++it)
			{
				ImGui::ProgressBar((*it).second * 0.01f, ImVec2(200, 0));

				ImGui::SameLine();

				ImGui::Text((*it).first.c_str());
			}

			ImGui::Separator();

			last_window_size = float2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
		}

		ImGui::End();
	}

	processes.clear();
}

void ProgressWindow::AddProcess(std::string name, float process)
{
	processes[name] = process;
}
