#include "HierarchyWindow.h"
#include "App.h"
#include "ModuleGameObject.h"
#include "GameObject.h"
#include "ModuleInput.h"
#include "GameObjectAbstraction.h"
#include "ModuleScene.h"
#include "ResourceScene.h"
#include "Scene.h"
#include "ModuleAssets.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

HierarchyWindow::HierarchyWindow()
{
}

HierarchyWindow::~HierarchyWindow()
{
}

void HierarchyWindow::Start()
{
	font = App->editor->GetLoadedFont("RobotoMedium_15");
	scenes_font = App->editor->GetLoadedFont("RobotoMedium_20");
}

void HierarchyWindow::CleanUp()
{
}

void HierarchyWindow::DrawEditor()
{
	DrawMenuBar();
	
	ImGui::PushFont(font);

	Scene* root_scene = App->gameobject->GetRootScene();

	uint scene_count = 0;
	uint go_count = 0;
	uint height_count = 0;

	height_count = ImGui::GetCursorPosY() + 60;

	DrawScene(root_scene, scene_count, go_count, height_count);

	++scene_count;

	std::vector<Scene*> sub_scenes = App->gameobject->GetSubScenes();

	for (std::vector<Scene*>::iterator it = sub_scenes.begin(); it != sub_scenes.end(); ++it)
	{
		Scene* curr_sub_scene = (*it);

		DrawScene(curr_sub_scene, scene_count, go_count, height_count);

		++scene_count;
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
		if (ImGui::Button("New Sub Scene"))
		{
			App->gameobject->CreateSubScene();
		}

		if (ImGui::Button("New GameObject"))
		{
			App->gameobject->CreateGameObject();
		}

		ImGui::EndMenuBar();
	}
}

void HierarchyWindow::DrawScenesPopup(bool left_clicked, bool right_clicked, uint scene_count)
{
	if (right_clicked)
	{
		ImGui::OpenPopupOnItemClick("SceneHerarchyPopup", 1);
	}

	std::vector<Scene*> selected = App->gameobject->GetSelectedScenes();

	bool open_rename = false;

	if (ImGui::BeginPopupContextItem("SceneHerarchyPopup"))
	{
		if (selected.size() == 1)
		{
			if (ImGui::Button("Move Up"))
			{
				App->gameobject->ChangeScenePositionOnList(selected[0], scene_count - 2);

				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("Move Down"))
			{
				App->gameobject->ChangeScenePositionOnList(selected[0], scene_count + 1);

				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("Save"))
			{
				App->gameobject->ChangeScenePositionOnList(selected[0], scene_count - 2);

				ImGui::CloseCurrentPopup();
			}

			ImGui::Separator();

			if (ImGui::Button("Rename"))
			{
				ImGui::CloseCurrentPopup();
				open_rename = true;
			}
		}

		if (ImGui::Button("Delete"))
		{
			for (std::vector<Scene*>::iterator it = selected.begin(); it != selected.end(); ++it)
			{
				App->gameobject->DestroyScene(*it);
			}
		}

		ImGui::EndPopup();
	}

	if (open_rename)
	{
		ImGui::OpenPopup("RenamePopup");

		if (selected.size() > 0)
		{
			Scene* sel_scene = *selected.begin();

			int size = sel_scene->GetName().size();

			if (size > 50)
				size = 50;

			memset(change_name_tmp, 0, sizeof(char) * 50);

			strcpy_s(change_name_tmp, sizeof(char) * size + 1, sel_scene->GetName().c_str());
		}
	}

	if (ImGui::BeginPopup("RenamePopup"))
	{
		ImGui::Text("Name: ");

		ImGui::SameLine();

		ImGui::InputText("", change_name_tmp, sizeof(char) * 50, ImGuiInputTextFlags_AutoSelectAll);

		if (ImGui::Button("Accept"))
		{
			if (selected.size() > 0)
			{
				Scene* sel_scene = *selected.begin();

				sel_scene->SetName(change_name_tmp);
			}

			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void HierarchyWindow::SceneInput(Scene * scene, bool left_clicked, bool right_clicked)
{
	//If ctrl is pressed do multiselection

	int selected_count = App->gameobject->GetSelectedGameObjectsCount();

	if ((App->input->GetKeyRepeat(SDL_SCANCODE_LCTRL) || App->input->GetKeyRepeat(SDL_SCANCODE_RCTRL)) && ImGui::IsItemClicked(0))
	{
		if (!scene->GetSelected())
			App->gameobject->AddSceneToSelected(scene);
		else
			App->gameobject->RemoveSceneFromSelected(scene);

		disable_button_up = true;
	}

	// If shift is pressed do fill gap selection
	else if ((App->input->GetKeyRepeat(SDL_SCANCODE_LSHIFT) || App->input->GetKeyRepeat(SDL_SCANCODE_RSHIFT)) && ImGui::IsItemClicked(0))
	{

	}

	// Monoselection
	else
	{
		if ((left_clicked || right_clicked) && !scene->GetSelected())
		{
			App->gameobject->RemoveAllScenesFromSelected();
			App->gameobject->AddSceneToSelected(scene);

			disable_button_up = true;
		}
		else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && ImGui::IsItemHovered() && scene->GetSelected() && selected_count == 1)
		{
			if (!disable_button_up)
			{
				App->gameobject->RemoveSceneFromSelected(scene);
			}
			else
				disable_button_up = false;
		}
		else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && ImGui::IsItemHovered())
		{
			if (!disable_button_up)
			{
				if (selected_count > 1)
				{
					App->gameobject->RemoveAllScenesFromSelected();
					App->gameobject->AddSceneToSelected(scene);
				}
			}
			else
				disable_button_up = false;
		}

		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && !ImGui::IsAnyItemHovered() && ImGui::IsMouseHoveringWindow())
		{
			App->gameobject->RemoveAllScenesFromSelected();
		}
	}
}

void HierarchyWindow::DrawScene(Scene* scene, uint scene_count, uint & go_count, uint& height_count)
{
	if (scene != nullptr)
	{
		if (scene_count == 0)
		{
			ImGui::PushFont(scenes_font);

			ImGui::PushID(scene->GetUid().c_str());
			std::string curr_scene_text = "Current scene: " + scene->GetName();
			ImGui::Text(curr_scene_text.c_str());

			if (ImGui::SmallButton("Save"))
			{
				App->assets->CreateScene();
			}

			bool open_scene_rename = false;

			ImGui::SameLine();

			if (ImGui::SmallButton("Rename"))
			{				
				open_scene_rename = true;
			}

			if (open_scene_rename)
			{
				ImGui::OpenPopup("RenameScenePopup");
				
				int size = scene->GetName().size();

				if (size > 98)
					size = 98;

				memset(change_name_tmp, 0, sizeof(char) * 99);

				strcpy_s(change_name_tmp, sizeof(char) * size + 1, scene->GetName().c_str());
				
			}

			if (ImGui::BeginPopup("RenameScenePopup"))
			{
				ImGui::Text("Name: ");

				ImGui::SameLine();

				ImGui::InputText("", change_name_tmp, sizeof(char) * 50, ImGuiInputTextFlags_AutoSelectAll);

				if (ImGui::Button("Accept"))
				{
					scene->SetName(change_name_tmp);
					
					ImGui::CloseCurrentPopup();
				}

				ImGui::SameLine();

				if (ImGui::Button("Cancel"))
				{
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

			ImGui::PopFont();

			ImGui::Separator();
		}
		else
		{
			bool selected = scene->GetSelected();

			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(47.0f / 255.0f, 197.0f / 255.0f, 104.0f / 255.0f, 1));

			ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, height_count));
			ImGui::PushID(scene->GetUid().c_str());
			ImGui::Selectable(scene->GetName().c_str(), selected);

			ImGui::PopStyleColor();

			bool left_clicked = false;
			bool right_clicked = false;

			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
				left_clicked = true;

			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN)
				right_clicked = true;

			SceneInput(scene, left_clicked, right_clicked);

			DrawScenesPopup(left_clicked, right_clicked, scene_count);

			height_count += 20;
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObjects"))
			{
				std::vector<GameObject*> selected_gos = App->gameobject->GetSelectedGameObjects();
				for (std::vector<GameObject*>::iterator it = selected_gos.begin(); it != selected_gos.end(); ++it)
				{
					if ((*it)->GetScene() != scene)
						App->gameobject->SetGameObjectScene(scene, (*it));
				}

				dragging = false;
			}

			ImGui::EndDragDropTarget();
		}

		std::vector<GameObject*> root_game_objects = scene->GetRootGameObjects();

		uint root_index = 0;

		for (std::vector<GameObject*>::iterator it = root_game_objects.begin(); it != root_game_objects.end(); ++it)
		{
			DrawGameObjectRecursive(scene, (*it), root_index, go_count, height_count);

			++root_index;
		}

		ImGui::PopID();
	}
}

void HierarchyWindow::DrawGameObjectsPopup(bool left_clicked, bool right_clicked)
{
	if (right_clicked)
	{
		ImGui::OpenPopupOnItemClick("HerarchyPopup", 1);
	}

	bool open_rename = false;

	std::vector<GameObject*> selected = App->gameobject->GetSelectedGameObjects();

	if (ImGui::BeginPopupContextItem("HerarchyPopup"))
	{
		if (selected.size() == 1)
		{
			if (ImGui::Button("Rename"))
			{
				ImGui::CloseCurrentPopup();
				open_rename = true;
			}

			if (ImGui::Button("Duplicate"))
			{
				GameObjectAbstraction abs;
				abs.Abstract(selected);
				abs.DeAbstract();

				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("Create new Prefab"))
			{
				App->scene->CreateNewPrefab(selected[0]);

				ImGui::CloseCurrentPopup();
			}

			if (selected[0]->GetHasPrefab())
			{
				if (ImGui::Button("Update Prefab"))
				{
					App->scene->UpdatePrefab(selected[0]);

					ImGui::CloseCurrentPopup();
				}

				if (ImGui::Button("Update Gameobject from Prefab"))
				{
					App->scene->UpdateFromPrefab(selected[0]);

					ImGui::CloseCurrentPopup();
				}
			}
		}

		if (ImGui::Button("Delete"))
		{
			for (std::vector<GameObject*>::iterator it = selected.begin(); it != selected.end(); ++it)
			{
				App->gameobject->DestroyGameObject(*it);
			}
		}

		ImGui::EndPopup();
	}

	if (open_rename)
	{
		ImGui::OpenPopup("RenamePopup");

		if (selected.size() > 0)
		{
			GameObject* sel_go = *selected.begin();

			int size = sel_go->GetName().size();

			if (size > 98)
				size = 98;

			memset(change_name_tmp, 0, sizeof(char) * 99);

			strcpy_s(change_name_tmp, sizeof(char) * size + 1, sel_go->GetName().c_str());
		}
	}

	if (ImGui::BeginPopup("RenamePopup"))
	{
		ImGui::Text("Name: ");

		ImGui::SameLine();

		ImGui::InputText("", change_name_tmp, sizeof(char) * 50, ImGuiInputTextFlags_AutoSelectAll);

		if (ImGui::Button("Accept"))
		{
			if (selected.size() > 0)
			{
				GameObject* sel_go = *selected.begin();

				sel_go->SetName(change_name_tmp);
			}

			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void HierarchyWindow::GameObjectInput(GameObject* go, bool left_clicked, bool right_clicked)
{	
	//If ctrl is pressed do multiselection
	
	int selected_count = App->gameobject->GetSelectedGameObjectsCount();

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

			disable_button_up = true;
		}
		else if(App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && ImGui::IsItemHovered() && go->GetSelected() && selected_count == 1)
		{
			if (!disable_button_up)
			{				
				App->gameobject->RemoveGameObjectFromSelected(go);
			}
			else
				disable_button_up = false;
		}
		else if(App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && ImGui::IsItemHovered())
		{
			if (!disable_button_up)
			{
				if (selected_count > 1)
				{
					App->gameobject->RemoveAllGameObjectsFromSelected();
					App->gameobject->AddGameObjectToSelected(go);
				}
			}
			else
				disable_button_up = false;
		}

		if(App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && !ImGui::IsAnyItemHovered() && ImGui::IsMouseHoveringWindow())
		{
			App->gameobject->RemoveAllGameObjectsFromSelected();
		}
	}	
}

void HierarchyWindow::DrawGameObjectRecursive(Scene* scene, GameObject* go, uint child_index, uint& go_count, uint& height_count)
{
	if (go != nullptr && scene != nullptr)
	{
		uint flags = ImGuiTreeNodeFlags_OpenOnArrow;

		flags |= ImGuiTreeNodeFlags_DefaultOpen;

		if (go->GetChildsCount() == 0)
			flags |= ImGuiTreeNodeFlags_Leaf;

		if (go->GetSelected())
			flags |= ImGuiTreeNodeFlags_Selected;

		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, height_count));
		ImGui::PushID(go->GetUID().c_str());

		std::string go_text = go->GetName();

		if (go->GetPrefab())
		{
			go_text += " [P]";
		}

		bool opened = ImGui::TreeNodeEx(go_text.c_str(), flags);

		bool left_clicked = false;
		bool right_clicked = false;

		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			left_clicked = true;

		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN)
			right_clicked = true;
		
		// -----

		GameObjectInput(go, left_clicked, right_clicked);

		DrawGameObjectsPopup(left_clicked, right_clicked);

		uint height_before = height_count + 18;
		DragAndDropBeforeChilds(scene, go, child_index, height_before);

		// -----

		ImGui::PopID();

		++go_count;
		height_count += 20;

		if (opened)
		{
			std::vector<GameObject*> childs = go->GetChilds();
			for (int i = 0; i < childs.size(); ++i)
			{
				DrawGameObjectRecursive(scene, childs[i], i, go_count, height_count);
			}

			uint height_after = height_count - 18;
			DragAndDropAfterChilds(scene, go, child_index, height_count);

			ImGui::TreePop();
		}
	}
}

void HierarchyWindow::DragAndDropBeforeChilds(Scene* scene, GameObject * go, uint child_index, uint height_count)
{
	// GO slot become drag target
	uint drag_drop_flags = ImGuiDragDropFlags_SourceNoDisableHover;

	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		int size = sizeof(go);
		ImGui::SetDragDropPayload("GameObjects", go, size);

		std::vector<GameObject*> selected_gos = App->gameobject->GetSelectedGameObjects();

		for (std::vector<GameObject*>::iterator it = selected_gos.begin(); it != selected_gos.end(); ++it)
		{
			ImGui::Text((*it)->GetName().c_str());
		}

		dragging = true;

		ImGui::EndDragDropSource();
	}

	// GO slot become drop target
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObjects"))
		{
			std::vector<GameObject*> selected_gos = App->gameobject->GetSelectedGameObjects();

			for (std::vector<GameObject*>::iterator it = selected_gos.begin(); it != selected_gos.end(); ++it)
			{
				if ((*it)->GetScene() != go->GetScene())
					App->gameobject->SetGameObjectScene(go->GetScene(), (*it));

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
			ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, height_count - 20));
			ImGui::Button("", ImVec2(GetWindowSize().x - ImGui::GetCursorPos().x - 10, 2));

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObjects"))
				{
					std::vector<GameObject*> selected_gos = App->gameobject->GetSelectedGameObjects();
					for (std::vector<GameObject*>::iterator it = selected_gos.begin(); it != selected_gos.end(); ++it)
					{
						if ((*it)->GetScene() != scene)
							App->gameobject->SetGameObjectScene(scene, (*it));

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
				ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, height_count));
				ImGui::Button("", ImVec2(GetWindowSize().x - ImGui::GetCursorPos().x - 10, 2));

				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObjects"))
					{
						std::vector<GameObject*> selected_gos = App->gameobject->GetSelectedGameObjects();
						for (std::vector<GameObject*>::iterator it = selected_gos.begin(); it != selected_gos.end(); ++it)
						{
							if ((*it)->GetScene() != scene)
								App->gameobject->SetGameObjectScene(scene, (*it));

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

void HierarchyWindow::DragAndDropAfterChilds(Scene* scene, GameObject * go, uint child_index, uint height_count)
{
	if (go->GetChildsCount() > 0 && go->GetParent() == nullptr && dragging)
	{
		ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x, height_count));
		ImGui::Button("", ImVec2(GetWindowSize().x - ImGui::GetCursorPos().x - 10, 2));

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObjects"))
			{
				std::vector<GameObject*> selected_gos = App->gameobject->GetSelectedGameObjects();
				for (std::vector<GameObject*>::iterator it = selected_gos.begin(); it != selected_gos.end(); ++it)
				{
					if ((*it)->GetScene() != scene)
						App->gameobject->SetGameObjectScene(scene, (*it));

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

