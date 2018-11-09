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
}

void HierarchyWindow::CleanUp()
{
}

void HierarchyWindow::DrawEditor()
{
	DrawMenuBar();

	std::vector<GameObject*> root_game_objects = App->gameobject->GetRootGameObjects();
	
	int index = 0;

	for (std::vector<GameObject*>::iterator it = root_game_objects.begin(); it != root_game_objects.end(); ++it)
	{
		DrawGameObjectRecursive((*it), true);
	}
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

void HierarchyWindow::GameObjectInput(GameObject* go)
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
		if (ImGui::IsItemClicked(0) && !go->GetSelected())
		{
			App->gameobject->RemoveAllGameObjectsFromSelected();
			App->gameobject->AddGameObjectToSelected(go);
		}
		else if(ImGui::IsItemClicked(1) && go->GetSelected())
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

void HierarchyWindow::DrawGameObjectRecursive(GameObject* go, bool is_root)
{
	if (go != nullptr)
	{
		uint flags = ImGuiTreeNodeFlags_OpenOnArrow;

		flags |= ImGuiTreeNodeFlags_DefaultOpen;

		if (go->GetChildsCount() == 0)
			flags |= ImGuiTreeNodeFlags_Leaf;

		if (go->GetSelected())
			flags |= ImGuiTreeNodeFlags_Selected;

		ImGui::PushID(go->GetUID().c_str());
		bool opened = ImGui::TreeNodeEx(go->GetName(), flags);

		// -----


		GameObjectInput(go);

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

			ImGui::EndDragDropSource();
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("test"))
			{
				std::vector<GameObject*> selected_gos = App->gameobject->GetSelectedGameObjects();

				for (std::vector<GameObject*>::iterator it = selected_gos.begin(); it != selected_gos.end(); ++it)
				{
					(*it)->SetParent(go);
				}
			}
			ImGui::EndDragDropTarget();
		}

		if (is_root)
		{
			ImGui::Separator();

			//if (ImGui::BeginDragDropTarget())
			//{
			//	if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("test"))
			//	{
			//		if (go_to_drag != nullptr)
			//		{
			//			std::string uid = (const char*)payload->Data;

			//			go_to_drag->SetParent(nullptr);

			//			go_to_drag = nullptr;
			//		}
			//	}
			//	ImGui::EndDragDropTarget();
			//}
		}

		// -----

		ImGui::PopID();

		if (opened)
		{
			int childs_count = go->GetChildsCount();

			for (int i = 0; i < childs_count; ++i)
			{
				DrawGameObjectRecursive(go->GetChild(i));
			}

			ImGui::TreePop();
		}
	}
}
