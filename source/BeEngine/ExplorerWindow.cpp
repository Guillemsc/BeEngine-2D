#include "ExplorerWindow.h"
#include "Event.h"
#include "App.h"
#include "ModuleResource.h"
#include "ModuleEvent.h"
#include "ModuleProject.h"
#include "ModuleInput.h"
#include "ModuleScripting.h"
#include "ScriptingObjectSolutionManager.h"

ExplorerWindow::ExplorerWindow()
{
}

ExplorerWindow::~ExplorerWindow()
{
}

void ExplorerWindow::OnEvent(Event * ev)
{
	switch (ev->GetType())
	{
	case EventType::WATCH_FILE_FOLDER:

		update_folders = true;

		break;
	case EventType::PROJECT_SELECTED:

		update_folders = true;

		break;
	default:
		break;
	}
}

void ExplorerWindow::Start()
{
	App->event->Suscribe(std::bind(&ExplorerWindow::OnEvent, this, std::placeholders::_1), EventType::WATCH_FILE_FOLDER);
	App->event->Suscribe(std::bind(&ExplorerWindow::OnEvent, this, std::placeholders::_1), EventType::PROJECT_SELECTED);
}

void ExplorerWindow::CleanUp()
{
	ClearFloldersAndFiles();
}

void ExplorerWindow::DrawEditor()
{
	if (update_folders)
	{
		UpdateFoldersAndFiles();

		update_folders = false;
	}

	if (App->project->GetCurrProjectIsSelected())
	{
		ImGui::Columns(2);

		DrawFoldersColumn();

		ImGui::NextColumn();

		DrawFilesColumn();
	}
}

ImGuiWindowFlags ExplorerWindow::GetWindowFlags()
{
	return ImGuiWindowFlags_MenuBar;
}

void ExplorerWindow::UpdateFoldersAndFiles()
{
	ClearFloldersAndFiles();

	folder_tree = App->file_system->GetFilesAndFoldersTree(App->resource->GetAssetsPath().c_str());
	
	cur_files.clear();
	
	std::vector<std::string> selected_folder_files_paths = App->file_system->GetFilesAndFoldersInPath(App->resource->GetCurrentAssetsPath().c_str());
	
	for (std::vector<std::string>::iterator it = selected_folder_files_paths.begin(); it != selected_folder_files_paths.end(); ++it)
	{
		ExplorerFile* ef = new ExplorerFile();
	
		if (!App->resource->IsMeta((*it).c_str()))
		{
			ef->dfp = App->file_system->DecomposeFilePath((*it).c_str());

			ef->selected = false;
	
			cur_files.push_back(ef);
		}
	}
}

void ExplorerWindow::ClearFloldersAndFiles()
{
	RemoveAllFromSelectedFiles();

	folder_tree.valid = false;

	for (std::vector<ExplorerFile*>::iterator it = cur_files.begin(); it != cur_files.end(); ++it)
	{
		RELEASE(*it);
	}

	cur_files.clear();
}

void ExplorerWindow::DrawFoldersColumn()
{
	DrawFoldersRecursive(folder_tree);
}

void ExplorerWindow::DrawFilesColumn()
{
	uint selected_files_count = selected_files.size();

	for (std::vector<ExplorerFile*>::iterator it = cur_files.begin(); it != cur_files.end(); ++it)
	{
		//ImGui::PushID(curr_ef.dfp.file_path.c_str());

		ExplorerFile* curr_file = (*it);

		std::string name = curr_file->dfp.file_name + "." + curr_file->dfp.file_extension_lower_case;

		uint flags = ImGuiTreeNodeFlags_Leaf;

		if (curr_file->selected)
			flags |= ImGuiTreeNodeFlags_Selected;

		bool opened = ImGui::TreeNodeEx(name.c_str(), flags);

		bool left_clicked = false;
		bool right_clicked = false;

		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			left_clicked = true;

		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN)
			right_clicked = true;

		// Input
		if ((App->input->GetKeyRepeat(SDL_SCANCODE_LCTRL) || App->input->GetKeyRepeat(SDL_SCANCODE_RCTRL)) && ImGui::IsItemClicked(0))
		{
			if (!curr_file->selected)
				AddToSelectedFiles(curr_file);
			else
				RemoveFromSelectedFiles(curr_file);

			disable_button_up = true;
		}

		// If shift is pressed do fill gap selection
		else if ((App->input->GetKeyRepeat(SDL_SCANCODE_LSHIFT) || App->input->GetKeyRepeat(SDL_SCANCODE_RSHIFT)) && ImGui::IsItemClicked(0))
		{

		}
		// Monoselection
		else
		{
			if ((left_clicked || right_clicked) && !curr_file->selected)
			{
				RemoveAllFromSelectedFiles();
				AddToSelectedFiles(*it);

				disable_button_up = true;
			}
			else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && ImGui::IsItemHovered() && curr_file->selected && selected_files_count == 1)
			{
				if (!disable_button_up)
				{
					RemoveFromSelectedFiles(*it);
				}
				else
					disable_button_up = false;
			}
			else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && ImGui::IsItemHovered())
			{
				if (!disable_button_up)
				{
					if (selected_files_count > 1)
					{
						RemoveAllFromSelectedFiles();
						AddToSelectedFiles(*it);
					}
				}
				else
					disable_button_up = false;
			}

			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && !ImGui::IsAnyItemHovered() && ImGui::IsMouseHoveringWindow())
			{
				RemoveAllFromSelectedFiles();
			}

			DrawFilesPopup(left_clicked, right_clicked);

			if (opened)
			{
				ImGui::TreePop();
			}
		}
	}
}

void ExplorerWindow::DrawFoldersRecursive(const Folder & folder)
{
	uint flags = ImGuiTreeNodeFlags_OpenOnArrow;

	if (folder.folders.size() == 0)
		flags |= ImGuiTreeNodeFlags_Leaf;

	bool draw = true;

	if (App->resource->GetAssetsPath().compare(folder.folder_path) == 0)
	{
		ImGui::SetNextTreeNodeOpen(true);
	}

	ImGui::PushID(folder.folder_path.c_str());
	bool opened = ImGui::TreeNodeEx(folder.folder_name.c_str(), flags);

	bool left_clicked = false;
	bool right_clicked = false;

	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
		left_clicked = true;

	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN)
		right_clicked = true;

	FoldersInput(folder.folder_path, left_clicked, right_clicked);

	DrawFilesPopup(left_clicked, right_clicked);

	ImGui::PopID();

	if (opened)
	{
		for (std::vector<Folder>::const_iterator it = folder.folders.begin(); it != folder.folders.end(); ++it)
		{
			DrawFoldersRecursive(*it);
		}

		ImGui::TreePop();
	}
}

void ExplorerWindow::SetSelectedFolderTree(const char * path)
{
	if (App->file_system->FolderExists(path))
	{
		App->resource->SetCurrentAssetsPath(path);
	}
}

void ExplorerWindow::FoldersInput(const std::string & folder, bool left_clicked, bool right_clicked)
{
	if (left_clicked)
	{
		SetSelectedFolderTree(folder.c_str());

		update_folders = true;
	}
}

void ExplorerWindow::DrawFilesPopup(bool left_clicked, bool right_clicked)
{
	std::vector<ExplorerFile*> selected = GetSelectedFiles();

	if (right_clicked)
	{
		ImGui::OpenPopupOnItemClick("FilesPopup", 1);
	}

	bool open_rename = false;

	if (ImGui::BeginPopupContextItem("FilesPopup"))
	{
		if (selected.size() == 1)
		{
			DecomposedFilePath selected_file = selected[0]->dfp;

			if (selected_file.file_extension_lower_case == "cs")
			{
				if (ImGui::Button("Edit Script"))
				{
					ImGui::CloseCurrentPopup();
					App->scripting->solution_manager->OpenSolutionWithExternalProgram();
				}
			}

			if (ImGui::Button("Show in Explorer"))
			{
				App->OpenFolder(selected_file.path.c_str());
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::Button("Rename"))
			{
				open_rename = true;
				ImGui::CloseCurrentPopup();
			}
		}


		ImGui::EndPopup();
	}

	if (open_rename)
	{
		ImGui::OpenPopup("RenamePopup");

		if (selected.size() > 0)
		{
			std::string name = (*selected.begin())->dfp.file_name;

			int size = name.size();

			if (size > 50)
				size = 50;

			memset(change_name_tmp, 0, sizeof(char) * 50);

			strcpy_s(change_name_tmp, sizeof(char) * size + 1, name.c_str());
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
				App->resource->RenameAsset(selected[0]->dfp.file_path.c_str(), change_name_tmp);
				update_folders = true;
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

void ExplorerWindow::AddToSelectedFiles(ExplorerFile* add)
{	
	bool already_selected = false;
	std::vector<ExplorerFile*> files = selected_files;
	for (std::vector<ExplorerFile*>::iterator it = files.begin(); it != files.end(); ++it)
	{
		if (add->dfp.file_path.compare((*it)->dfp.file_path) == 0)
		{
			already_selected = true;
			break;
		}
	}

	if (!already_selected)
	{
		add->selected = true;
		selected_files.push_back(add);
	}
	
}

void ExplorerWindow::RemoveFromSelectedFiles(ExplorerFile* add)
{
	for (std::vector<ExplorerFile*>::iterator it = selected_files.begin(); it != selected_files.end(); ++it)
	{
		if (add->dfp.file_path.compare((*it)->dfp.file_path) == 0)
		{
			add->selected = false;
			selected_files.erase(it);
			break;
		}
	}
}

void ExplorerWindow::RemoveAllFromSelectedFiles()
{
	for (std::vector<ExplorerFile*>::iterator it = selected_files.begin(); it != selected_files.end(); ++it)
	{
		(*it)->selected = false;
	}

	selected_files.clear();
}

std::vector<ExplorerFile*> ExplorerWindow::GetSelectedFiles()
{
	return selected_files;
}

ExplorerFile::ExplorerFile()
{
}
