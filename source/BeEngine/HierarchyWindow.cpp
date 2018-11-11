#include "HierarchyWindow.h"
#include "App.h"
#include "ModuleGameObject.h"
#include "GameObject.h"
#include "ModuleInput.h"

HierarchyWindow::HierarchyWindow()
{
}

HierarchyWindow::~HierarchyWindow()
{
}

void HierarchyWindow::Start()
{
	font = App->editor->GetLoadedFont("RobotoMedium_15");
}

void HierarchyWindow::CleanUp()
{
}

void HierarchyWindow::DrawEditor()
{
	DrawMenuBar();

	std::vector<GameObject*> root_game_objects = App->gameobject->GetRootGameObjects();
	
	ImGui::PushFont(font);

	uint go_count = 0;

	uint root_index = 0;
	for (std::vector<GameObject*>::iterator it = root_game_objects.begin(); it != root_game_objects.end(); ++it)
	{
		DrawGameObjectRecursive((*it), root_index, go_count);

		++root_index;
	}

	ImGui::PopFont();
}

ImGuiWindowFlags HierarchyWindow::GetWindowFlags()
{
	return ImGuiWindowFlags_MenuBar;
}

void HierarchyWindow::DrawMenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::Button("New GameObject"))
		{
			App->gameobject->CreateGameObject();
		}

		ImGui::EndMenuBar();
	}
}

void HierarchyWindow::DrawGameObjectsPopup(GameObject * go, bool left_clicked, bool right_clicked)
{
	if (right_clicked)
	{
		ImGui::OpenPopupOnItemClick("HerarchyPopup", 1);
	}

	if (ImGui::BeginPopupContextItem("HerarchyPopup"))
	{
		if (ImGui::Button("Rename"))
		{

		}

		ImGui::EndPopup();
	}
}

void HierarchyWindow::GameObjectInput(GameObject* go, bool left_clicked, bool right_clicked)
{	
	//If ctrl is pressed do multiselection
	
	if ((App->input->GetKeyRepeat(SDL_SCANCODE_LCTRL) || App->input->GetKeyRepeat(SDL_SCANCODE_RCTRL)) && ImGui::IsItemClicked(0))
	{
		if(!go->GetSelected())
			App->gameobject->AddGameObjectToSelected(go);
		else
			App->gameobject->RemoveGameObjectFromSelected(go);

		disable_button_up = true;
	}

	// If shift is pressed do fill gap selection
	else if ((App->input->GetKeyRepeat(SDL_SCANCODE_LSHIFT) || App->input->GetKeyRepeat(SDL_SCANCODE_RSHIFT)) && ImGui::IsItemClicked(0))
	{

	}
	
	// Monoselection
	else
	{
		if ((left_clicked || right_clicked) && !go->GetSelected())
		{
			App->gameobject->RemoveAllGameObjectsFromSelected();
			App->gameobject->AddGameObjectToSelected(go);
		}
		else if(left_clicked && go->GetSelected())
		{
			if (App->gameobject->GetSelectedGameObjectsCount() == 1)
			{
				App->gameobject->RemoveGameObjectFromSelected(go);
			}
		}
		else if(App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && ImGui::IsItemHovered())
		{
			if (!disable_button_up)
			{
				if (App->gameobject->GetSelectedGameObjectsCount() > 1)
				{
					App->gameobject->RemoveAllGameObjectsFromSelected();
					App->gameobject->AddGameObjectToSelected(go);
				}
			}
			else
				disable_button_up = false;
		}

		if(App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && !ImGui::IsAnyItemHovered())
		{
			App->gameobject->RemoveAllGameObjectsFromSelected();
		}
	}	
}

void HierarchyWindow::DrawGameObjectRecursive(GameObject* go, uint child_index, uint& go_count)
{
	if (go != nullptr)
	{
		uint flags = ImGuiTreeNodeFlags_OpenOnArrow;

		flags |= ImGuiTreeNodeFlags_DefaultOpen;

		if (go->GetChildsCount() == 0)
			flags |= ImGuiTreeNodeFlags_Leaf;

		if (go->GetSelected())
			flags |= ImGuiTreeNodeFlags_Selected;

		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, 30 + ( go_count * 20)));
		ImGui::PushID(go->GetUID().c_str());
		bool opened = ImGui::TreeNodeEx(go->GetName(), flags);

		bool left_clicked = false;
		bool right_clicked = false;

		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			left_clicked = true;

		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN)
			right_clicked = true;
		
		// -----

		GameObjectInput(go, left_clicked, right_clicked);

		DrawGameObjectsPopup(go, left_clicked, right_clicked);

		DragAndDropBeforeChilds(go, child_index, go_count);

		// -----

		ImGui::PopID();

		++go_count;

		if (opened)
		{
			std::vector<GameObject*> childs = go->GetChilds();
			for (int i = 0; i < childs.size(); ++i)
			{
				DrawGameObjectRecursive(childs[i], i, go_count);
			}

			DragAndDropAfterChilds(go, child_index, go_count);

			ImGui::TreePop();
		}
	}
}

void HierarchyWindow::DragAndDropBeforeChilds(GameObject * go, uint child_index, uint & go_count)
{
	// GO slot become drag target
	uint drag_drop_flags = ImGuiDragDropFlags_SourceNoDisableHover;

	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		int size = sizeof(go);
		ImGui::SetDragDropPayload("test", go, size);

		std::vector<GameObject*> selected_gos = App->gameobject->GetSelectedGameObjects();

		for (std::vector<GameObject*>::iterator it = selected_gos.begin(); it != selected_gos.end(); ++it)
		{
			ImGui::Text((*it)->GetName());
		}

		dragging = true;

		ImGui::EndDragDropSource();
	}

	// GO slot become drop target
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("test"))
		{
			std::vector<GameObject*> selected_gos = App->gameobject->GetSelectedGameObjects();

			for (std::vector<GameObject*>::iterator it = selected_gos.begin(); it != selected_gos.end(); ++it)
			{
				(*it)->SetParent(go);
			}

			dragging = false;
		}
		ImGui::EndDragDropTarget();
	}

	if (dragging)
	{
		// Slot in between on top of go become drop target
		if (child_index == 0)
		{
			ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, 27 + (go_count * 20)));
			ImGui::Button("", ImVec2(GetWindowSize().x - ImGui::GetCursorPos().x - 10, 2));

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("test"))
				{
					std::vector<GameObject*> selected_gos = App->gameobject->GetSelectedGameObjects();
					for (std::vector<GameObject*>::iterator it = selected_gos.begin(); it != selected_gos.end(); ++it)
					{
						if (go->GetParent() == nullptr)
						{
							(*it)->SetParent(nullptr);

							App->gameobject->ChangeGameObjectPositionOnRootList((*it), child_index);
						}
						else
						{
							(*it)->SetParent(go->GetParent());
							App->gameobject->ChangeGameObjectPositionOnParentChildren((*it), child_index);
						}
					}

					dragging = false;
				}
				ImGui::EndDragDropTarget();
			}
		}

		// Slot in between on down of go become drop target
		if (go->GetChildsCount() == 0)
		{
			if (go->GetParent() == nullptr || (go->GetParent() != nullptr && child_index != go->GetParent()->GetChildsCount() - 1))
			{
				ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, 48 + (go_count * 20)));
				ImGui::Button("", ImVec2(GetWindowSize().x - ImGui::GetCursorPos().x - 10, 2));

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("test"))
					{
						std::vector<GameObject*> selected_gos = App->gameobject->GetSelectedGameObjects();
						for (std::vector<GameObject*>::iterator it = selected_gos.begin(); it != selected_gos.end(); ++it)
						{
							if (go->GetParent() == nullptr)
							{
								(*it)->SetParent(nullptr);

								App->gameobject->ChangeGameObjectPositionOnRootList((*it), child_index + 1);
							}
							else
							{
								(*it)->SetParent(go->GetParent());
								App->gameobject->ChangeGameObjectPositionOnParentChildren((*it), child_index + 1);
							}
						}

						dragging = false;
					}
					ImGui::EndDragDropTarget();
				}
			}
		}
	}

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
	{
		dragging = false;
	}
}

void HierarchyWindow::DragAndDropAfterChilds(GameObject * go, uint child_index, uint & go_count)
{
	if (go->GetChildsCount() > 0 && go->GetParent() == nullptr && dragging)
	{
		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, 27 + (go_count * 20)));
		ImGui::Button("", ImVec2(GetWindowSize().x - ImGui::GetCursorPos().x - 10, 2));

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("test"))
			{
				std::vector<GameObject*> selected_gos = App->gameobject->GetSelectedGameObjects();
				for (std::vector<GameObject*>::iterator it = selected_gos.begin(); it != selected_gos.end(); ++it)
				{
					if (go->GetParent() == nullptr)
					{
						(*it)->SetParent(nullptr);

						App->gameobject->ChangeGameObjectPositionOnRootList((*it), child_index + 1);
					}
					else
					{
						(*it)->SetParent(go->GetParent());
						App->gameobject->ChangeGameObjectPositionOnParentChildren((*it), child_index + 1);
					}
				}

				dragging = false;
			}
			ImGui::EndDragDropTarget();
		}
	}
}

