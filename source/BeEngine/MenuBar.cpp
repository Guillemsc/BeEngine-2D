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

		}

		if (ImGui::MenuItem("Edit"))
		{

		}

		if (ImGui::MenuItem("Assets"))
		{

		}

		if (ImGui::MenuItem("GameObjects"))
		{

		}

		if (ImGui::MenuItem("Components"))
		{

		}

		if (ImGui::MenuItem("Windows"))
		{

		}

		if (ImGui::MenuItem("About"))
		{

		}

		ImGui::EndMainMenuBar();
	}
	ImGui::PopFont();
}
