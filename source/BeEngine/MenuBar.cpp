#include "MenuBar.h"
#include "App.h"
#include "ModuleEditor.h"
#include "imgui.h"

MenuBar::MenuBar()
{
}

MenuBar::~MenuBar()
{
}

void MenuBar::Start()
{
	font = App->editor->GetLoadedFont("RobotoMedium_16");

	memset(new_layout_name, 0, 50);
}

void MenuBar::CleanUp()
{
}

void MenuBar::DrawEditor()
{
	ImGui::PushFont(font);

	if (ImGui::BeginMainMenuBar())
	{

		if (ImGui::MenuItem("File"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::MenuItem("Edit"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::MenuItem("Assets"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::MenuItem("GameObjects"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::MenuItem("Components"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Windows"))
		{
			DrawWindowsEditor();

			ImGui::EndMenu();
		}

		if (ImGui::MenuItem("About"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Layouts"))
		{
			DrawLayoutsEditor();

			ImGui::EndMenu();
		}

		std::string fps = "Fps: " + std::to_string(App->profiler->GetFPS());
		ImGui::Text(fps.c_str());

		ImGui::EndMainMenuBar();
	}
	ImGui::PopFont();
}

void MenuBar::DrawLayoutsEditor()
{
	ImGui::Text("Current Layout: %s", App->editor->GetCurrentDockingProfile());

	ImGui::Separator();

	ImGui::Text("Create new Layout: ");

	ImGui::InputText("", new_layout_name, 50);
	if (ImGui::Button("Create"))
	{
		if (App->editor->CreateNewDockingProfile(new_layout_name))
		{
			App->editor->SetCurrentDockingProfile(new_layout_name);

			memset(new_layout_name, 0, 50);
		}
	}

	ImGui::Separator();

	ImGui::Text("Created Layouts: ");

	std::vector<std::string> profiles = App->editor->GetDockingProfiles();

	for (std::vector<std::string>::iterator it = profiles.begin(); it != profiles.end(); ++it)
	{
		const char* profile = (*it).c_str();

		ImGui::PushID(profile);

		if (App->editor->CanRemoveDockingProfile(profile))
		{
			if (ImGui::Button(profile))
			{
				App->editor->SaveCurrentDockingProfile();
				App->editor->SetCurrentDockingProfile(profile);
			}

			ImGui::SameLine();

			if (ImGui::Button("X"))
			{
				App->editor->RemoveDockingProfile(profile);
			}
		}

		ImGui::PopID();
	}
}

void MenuBar::DrawWindowsEditor()
{
	std::vector<EditorWindow*> editor_windows = App->editor->GetEditorWindows();

	for (std::vector<EditorWindow*>::iterator it = editor_windows.begin(); it != editor_windows.end(); ++it)
	{
		EditorWindow* curr_window = *it;

		ImGui::MenuItem(curr_window->GetName().c_str(), NULL, &curr_window->opened);
	}
}
