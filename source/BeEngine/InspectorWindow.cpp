#include "InspectorWindow.h"
#include "App.h"
#include "ModuleGameObject.h"
#include "GameObjectComponent.h"
#include "imgui.h"

InspectorWindow::InspectorWindow()
{
}

InspectorWindow::~InspectorWindow()
{
}

void InspectorWindow::Start()
{
}

void InspectorWindow::CleanUp()
{
}

void InspectorWindow::DrawEditor()
{
	std::vector<GameObject*> selected_gos = App->gameobject->GetSelectedGameObjects();

	if (selected_gos.size() == 1)
	{
		GameObject* selected_go = *selected_gos.begin();

		char name[51];
		strcpy_s(name, 51, selected_go->GetName().c_str());
		if (ImGui::InputText("", name, 50, ImGuiInputTextFlags_AutoSelectAll))
			selected_go->SetName(name);
	}

	ImGui::Separator();

	int count = 0;
	for (std::vector<GameObject*>::iterator it = selected_gos.begin(); it != selected_gos.end(); ++it)
	{
		GameObject* curr_game_object = (*it);

		std::vector<GameObjectComponent*> components = curr_game_object->GetComponents();

		ImGui::SameLine();

		if (count == 0)
		{
			for (std::vector<GameObjectComponent*>::iterator com = components.begin(); com != components.end(); ++com)
			{
				GameObjectComponent* curr_component = (*com);

				ImGui::Separator();

				if (ImGui::SmallButton("Delete Component"))
				{

				}

				ImGui::SameLine();

				if (ImGui::SmallButton("Move Up"))
				{

				}

				ImGui::SameLine();


				if (ImGui::SmallButton("Move Down"))
				{

				}

				if (ImGui::CollapsingHeader(curr_component->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::Indent(15);

					curr_component->EditorDraw();

					ImGui::Indent(-15);
				}

				ImGui::Separator();
			}
		}

		++count;
	}
}

ImGuiWindowFlags InspectorWindow::GetWindowFlags()
{
	return ImGuiWindowFlags();
}
