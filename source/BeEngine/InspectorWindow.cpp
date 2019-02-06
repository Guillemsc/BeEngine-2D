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
	float2 win_size = GetWindowSize();

	std::vector<GameObject*> selected_gos = App->gameobject->GetSelectedGameObjects();

	if (selected_gos.size() == 1)
	{
		GameObject* selected_go = *selected_gos.begin();

		char name[99];
		strcpy_s(name, 99, selected_go->GetName().c_str());
		if (ImGui::InputText("", name, 50, ImGuiInputTextFlags_AutoSelectAll))
			selected_go->SetName(name);

		ImGui::Separator();

		ImGui::Spacing();
	}

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

				ImGui::PushID(curr_component->GetUID().c_str());

				if (curr_component->GetCanDestroy())
				{
					if (ImGui::SmallButton("Delete"))
					{
						curr_game_object->DestroyComponent(curr_component);
					}
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

				ImGui::PopID();
			}
		}

		++count;
	}

	bool open_components_popup = false;

	if (selected_gos.size() > 0)
	{
		ImGui::Spacing();

		if (ImGui::Button("Add Component", ImVec2(win_size.x - 12, 30)))
		{
			open_components_popup = true;
		}
	}

	if (open_components_popup)
	{
		ImGui::OpenPopup("CreateComponentPopup");
	}

	DrawComponentsPopup(selected_gos);
}

ImGuiWindowFlags InspectorWindow::GetWindowFlags()
{
	return ImGuiWindowFlags();
}

void InspectorWindow::DrawComponentsPopup(const std::vector<GameObject*>& selected_gos)
{
	if (ImGui::BeginPopup("CreateComponentPopup"))
	{
		ImGui::Text("Components:");

		ImGui::Separator();

		std::map<ComponentType, std::string> components_type = App->gameobject->GetComponentsTypes();

		for (std::map<ComponentType, std::string>::iterator it = components_type.begin(); it != components_type.end(); ++it)
		{
			if(ImGui::Button((*it).second.c_str()))
			{
				for (std::vector<GameObject*>::const_iterator go = selected_gos.begin(); go != selected_gos.end(); ++go)
				{
					(*go)->CreateComponent((*it).first);
				}

				ImGui::CloseCurrentPopup();
				break;
			}
		}

		ImGui::EndPopup();
	}
}
