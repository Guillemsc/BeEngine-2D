#include "ExplorerWindow.h"
#include "Event.h"
#include "App.h"
#include "ModuleResource.h"
#include "ModuleEvent.h"
#include "ModuleProject.h"

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
		folder_tree = App->file_system->GetFilesAndFoldersTree(App->resource->GetAssetsPath().c_str());
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

void ExplorerWindow::DrawFoldersColumn()
{
	DrawFoldersRecursive(folder_tree);
}

void ExplorerWindow::DrawFilesColumn()
{
	for (std::vector<std::string>::iterator it = selected_folder_files_paths.begin(); it != selected_folder_files_paths.end(); ++it)
	{
		ImGui::Text((*it).c_str());
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
		selected_folder_files_paths = App->file_system->GetFilesAndFoldersInPath(path);

		App->resource->SetCurrentAssetsPath(path);
	}
}
