#include "MenuBar.h"
#include "imgui.h"

MenuBar::MenuBar()
{
}

MenuBar::~MenuBar()
{
}

void MenuBar::CleanUp()
{
}

void MenuBar::DrawEditor()
{
	ImGui::BeginMainMenuBar();


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
