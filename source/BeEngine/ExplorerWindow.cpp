#include "ExplorerWindow.h"
#include "Event.h"
#include "App.h"
#include "ModuleResource.h"
#include "ModuleEvent.h"
#include "ModuleProject.h"
#include "ModuleInput.h"

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
	folder_tree = App->file_system->GetFilesAndFoldersTree(App->resource->GetAssetsPath().c_str());
	
	cur_files.clear();
	
	std::vector<std::string> selected_folder_files_paths = App->file_system->GetFilesAndFoldersInPath(App->resource->GetCurrentAssetsPath().c_str());
	
	for (std::vector<std::string>::iterator it = selected_folder_files_paths.begin(); it != selected_folder_files_paths.end(); ++it)
	{
		ExplorerFile ef;
	
		if (!App->resource->IsMeta((*it).c_str()))
		{
			ef.dfp = App->file_system->DecomposeFilePath((*it).c_str());

			ef.selected = false;
	
			cur_files.push_back(ef);
		}
	}
}

void ExplorerWindow::DrawFoldersColumn()
{
	DrawFoldersRecursive(folder_tree);
}

void ExplorerWindow::DrawFilesColumn()
{
	for (std::vector<ExplorerFile>::iterator it = cur_files.begin(); it != cur_files.end(); ++it)
	{
		//ImGui::PushID(curr_ef.dfp.file_path.c_str());

		std::string name = (*it).dfp.file_name + "." + (*it).dfp.file_extension_lower_case;

		uint flags = ImGuiTreeNodeFlags_Leaf;

		if ((*it).selected)
			flags |= ImGuiTreeNodeFlags_Selected;

		bool opened = ImGui::TreeNodeEx(name.c_str(), flags);

		bool left_clicked = false;
		bool right_clicked = false;

		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
			left_clicked = true;

		if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN)
			right_clicked = true;

		if (left_clicked)
			(*it).selected = !(*it).selected;

		DrawFilesPopup(left_clicked, right_clicked);

		if(left_clicked)
			system("C:\\Users\\Guillem\\Desktop\\Test\\SolutionTest.sln");

		if (opened)
		{
			ImGui::TreePop();
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
	if (right_clicked)
	{
		ImGui::OpenPopupOnItemClick("FilesPopup", 1);
	}

	bool open_rename = false;

	if (ImGui::BeginPopupContextItem("FilesPopup"))
	{
		if (1)
		{
			if (ImGui::Button("Rename"))
			{
				ImGui::CloseCurrentPopup();
				open_rename = true;
			}
		}

		ImGui::EndPopup();
	}
}

std::vector<ExplorerFile> ExplorerWindow::GetSelectedFiles()
{
	std::vector<ExplorerFile> ret;

	for (std::vector<ExplorerFile>::iterator it = cur_files.begin(); it != cur_files.end(); ++it)
	{
		if ((*it).selected)
			ret.push_back((*it));
	}

	return ret;
}

ExplorerFile::ExplorerFile()
{
}
