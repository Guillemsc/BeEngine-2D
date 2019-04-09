#include "ExplorerWindow.h"
#include "Event.h"
#include "App.h"
#include "ModuleResource.h"
#include "ModuleEvent.h"
#include "ModuleProject.h"
#include "ModuleInput.h"
#include "ModuleScripting.h"
#include "ScriptingObjectSolutionManager.h"
#include "ScriptingObjectCompiler.h"
#include "ModuleAssets.h"
#include "InspectorWindow.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

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
		update_files = true;

		break;
	case EventType::PROJECT_SELECTED:

		update_folders = true;
		update_files = true;

		SetSelectedFolderTree(App->assets->GetAssetsPath().c_str());

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
	ClearFolders();
	ClearFiles();
}

void ExplorerWindow::DrawEditor()
{
	float2 win_size = GetWindowSize();


	if (update_folders)
	{
		UpdateFolders();

		update_folders = false;
	}

	if (update_files)
	{
		UpdateFiles();

		update_files = false;
	}

	if (App->project->GetCurrProjectIsSelected())
	{
		ImGui::BeginChild("FoldersChild", ImVec2(win_size.x * 0.3f, 0), true, ImGuiWindowFlags_HorizontalScrollbar);

		DrawFoldersColumn();

		DrawFoldersPopupExtern();

		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("FilesChild", ImVec2(0, 0), true, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);

		DrawFilesMenuBar();

		DrawFilesColumn();

		DrawFilesPopupExtern();

		ImGui::EndChild();
	}
}

ImGuiWindowFlags ExplorerWindow::GetWindowFlags()
{
	return ImGuiWindowFlags();
}

void ExplorerWindow::UpdateFolders()
{
	ClearFolders();

	Folder folder_tree = App->file_system->GetFilesAndFoldersTree(App->assets->GetAssetsPath().c_str());

	CreateExplorerFolderRecursive(folder_tree, nullptr);
}

void ExplorerWindow::ClearFolders()
{
	RemoveAllFromSelectedFolders();
	DestroyAllExplorerFolders();
}

void ExplorerWindow::UpdateFiles()
{
	ClearFiles();

	if (!App->file_system->FolderExists(App->assets->GetCurrentAssetsPath().c_str()))
		SetSelectedFolderTree(App->assets->GetAssetsPath().c_str());
	
	std::vector<std::string> selected_folder_files_paths = App->file_system->GetFilesInPath(App->assets->GetCurrentAssetsPath().c_str());

	for (std::vector<std::string>::iterator it = selected_folder_files_paths.begin(); it != selected_folder_files_paths.end(); ++it)
	{
		ExplorerFile* ef = new ExplorerFile();

		if (!App->assets->IsMeta((*it).c_str()))
		{
			ef->dfp = App->file_system->DecomposeFilePath((*it).c_str());

			ef->selected = false;

			ef->resource = App->resource->GetResourceFromAssetFile((*it).c_str());

			curr_files.push_back(ef);
		}
	}

	SetSelectedFolderTree(App->assets->GetCurrentAssetsPath().c_str());
}

void ExplorerWindow::ClearFiles()
{
	RemoveAllFromSelectedFiles();

	for (std::vector<ExplorerFile*>::iterator it = curr_files.begin(); it != curr_files.end(); ++it)
	{
		RELEASE(*it);
	}

	curr_files.clear();
}

void ExplorerWindow::DrawFilesMenuBar()
{
	if (ImGui::BeginMenuBar())
	{
		ImGui::Text(files_curr_path.c_str());

		ImGui::EndMenuBar();
	}
}

void ExplorerWindow::DrawFoldersColumn()
{
	for (std::vector<ExplorerFolder*>::iterator it = curr_folders.begin(); it != curr_folders.end(); ++it)
	{
		DrawFoldersRecursive((*it));
	}
}

void ExplorerWindow::DrawFilesColumn()
{
	uint selected_files_count = selected_files.size();

	for (std::vector<ExplorerFile*>::iterator it = curr_files.begin(); it != curr_files.end(); ++it)
	{
		ExplorerFile* curr_file = (*it);

		std::string name = "";
		
		if (!curr_file->dfp.its_folder)
			name = curr_file->dfp.file_name + "." + curr_file->dfp.file_extension_lower_case;
		else
			name = curr_file->dfp.folder_name;

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

		FilesInput(curr_file, left_clicked, right_clicked);

		DrawFilesPopupIntern(left_clicked, right_clicked);

		FilesDragAndDrop(curr_file);

		if (opened)
		{
			ImGui::TreePop();
		}
	}
}

void ExplorerWindow::CreateExplorerFolderRecursive(const Folder & to_create, ExplorerFolder * parent)
{
	ExplorerFolder* ef = new ExplorerFolder();
	ef->dfp = App->file_system->DecomposeFilePath(to_create.folder_path);
	ef->selected = false;
	ef->folder_name = to_create.folder_name;

	if (parent != nullptr)
	{
		parent->childs.push_back(ef);
	}
	else
	{
		curr_folders.push_back(ef);
	}

	for (std::vector<Folder>::const_iterator it = to_create.folders.begin(); it != to_create.folders.end(); ++it)
	{
		CreateExplorerFolderRecursive((*it), ef);
	}
}

void ExplorerWindow::DestroyAllExplorerFolders()
{
	std::vector<ExplorerFolder*> to_destroy;

	for (std::vector<ExplorerFolder*>::iterator it = curr_folders.begin(); it != curr_folders.end(); ++it)
	{
		to_destroy.push_back((*it));
	}

	curr_folders.clear();

	while (!to_destroy.empty())
	{
		ExplorerFolder* curr_folder = *to_destroy.begin();

		to_destroy.erase(to_destroy.begin());

		for (std::vector<ExplorerFolder*>::iterator it = curr_folder->childs.begin(); it != curr_folder->childs.end(); ++it)
		{
			to_destroy.push_back((*it));
		}

		RELEASE(curr_folder);
	}
}

void ExplorerWindow::DrawFoldersRecursive(ExplorerFolder* folder)
{
	uint flags = ImGuiTreeNodeFlags_OpenOnArrow;

	if (folder->childs.size() == 0)
		flags |= ImGuiTreeNodeFlags_Leaf;

	if (folder->selected)
		flags |= ImGuiTreeNodeFlags_Selected;

	bool draw = true;

	if (App->assets->GetAssetsPath().compare(folder->dfp.path) == 0)
	{
		ImGui::SetNextTreeNodeOpen(true);
	}

	ImGui::PushID(folder->dfp.path.c_str());
	bool opened = ImGui::TreeNodeEx(folder->folder_name.c_str(), flags);

	bool left_clicked = false;
	bool right_clicked = false;

	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
		left_clicked = true;

	if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN)
		right_clicked = true;

	FoldersInput(folder, left_clicked, right_clicked);

	DrawFoldersPopupIntern(folder, left_clicked, right_clicked);

	FoldersDragAndDrop(folder);

	ImGui::PopID();

	if (opened)
	{
		for (std::vector<ExplorerFolder*>::const_iterator it = folder->childs.begin(); it != folder->childs.end(); ++it)
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
		App->assets->SetCurrentAssetsPath(path);

		std::string assets_path_parent = App->file_system->FolderParent(App->assets->GetAssetsPath().c_str());
		files_curr_path = App->file_system->SubstractFolder(App->assets->GetCurrentAssetsPath(), assets_path_parent);

		update_files = true;
	}
}

void ExplorerWindow::FoldersInput(ExplorerFolder* folder, bool left_clicked, bool right_clicked)
{
	uint selected_folders_count = selected_folders.size();

	// Input
	if ((App->input->GetKeyRepeat(SDL_SCANCODE_LCTRL) || App->input->GetKeyRepeat(SDL_SCANCODE_RCTRL)) && ImGui::IsItemClicked(0))
	{
		if (!folder->selected)
			AddToSelectedFolders(folder);
		else
			RemoveFromSelectedFolders(folder);

		disable_button_up = true;
	}

	// If shift is pressed do fill gap selection
	else if ((App->input->GetKeyRepeat(SDL_SCANCODE_LSHIFT) || App->input->GetKeyRepeat(SDL_SCANCODE_RSHIFT)) && ImGui::IsItemClicked(0))
	{

	}
	// Monoselection
	else
	{
		if ((left_clicked || right_clicked) && !folder->selected)
		{
			RemoveAllFromSelectedFolders();
			AddToSelectedFolders(folder);

			SetSelectedFolderTree(folder->dfp.path.c_str());

			disable_button_up = true;
		}
		else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && ImGui::IsItemHovered() && folder->selected && selected_folders_count == 1)
		{
			if (!disable_button_up)
			{
				RemoveFromSelectedFolders(folder);
			}
			else
				disable_button_up = false;
		}
		else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && ImGui::IsItemHovered())
		{
			if (!disable_button_up)
			{
				if (selected_folders_count > 1)
				{
					RemoveAllFromSelectedFolders();
					AddToSelectedFolders(folder);
				}
			}
			else
				disable_button_up = false;
		}

		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && !ImGui::IsAnyItemHovered() && ImGui::IsMouseHoveringWindow())
		{
			RemoveAllFromSelectedFolders();
		}
	}
}

void ExplorerWindow::DrawFoldersPopupIntern(ExplorerFolder* folder, bool left_clicked, bool right_clicked)
{
	bool folder_is_root = false;

	if (folder->dfp.path.compare(App->assets->GetAssetsPath()) == 0)
		folder_is_root = true;
	

	if (right_clicked)
	{
		ImGui::OpenPopupOnItemClick("FoldersPopup", 1);
	}

	bool open_folder_create = false;
	bool open_rename_folder = false;

	if (ImGui::BeginPopupContextItem("FoldersPopup", 1))
	{
		if (selected_folders.size() > 0 && !folder_is_root)
		{
			if (ImGui::Button("Delete"))
			{
				for (std::vector<ExplorerFolder*>::iterator it = selected_folders.begin(); it != selected_folders.end(); ++it)
				{
					App->assets->DeleteAssetsFolder((*it)->dfp.path.c_str());
				}

				update_folders = true;
				update_files = true;

				ImGui::CloseCurrentPopup();
			}
		}

		if (selected_folders.size() == 1)
		{
			if (!folder_is_root)
			{
				if (ImGui::Button("Rename Folder"))
				{
					open_rename_folder = true;

					ImGui::CloseCurrentPopup();
				}
			}

			if (ImGui::Button("Create Folder"))
			{
				open_folder_create = true;

				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::EndPopup();
	}

	if (open_folder_create)
	{
		ImGui::OpenPopup("FoldersCreatePopup");

		if (selected_folders.size() > 0)
		{
			memset(name_tmp, 0, sizeof(char) * 50);
		}
	}

	if (ImGui::BeginPopup("FoldersCreatePopup"))
	{
		ImGui::Text("Folder Name: ");

		ImGui::SameLine();

		ImGui::InputText("", name_tmp, sizeof(char) * 50, ImGuiInputTextFlags_AutoSelectAll);

		if (ImGui::Button("Accept"))
		{
			if (selected_folders.size() > 0)
			{
				std::string name = name_tmp;

				if (name.size() > 0)
				{
					App->assets->CreateAssetsFolder(selected_folders[0]->dfp.path.c_str(), name_tmp);

					update_folders = true;

					ImGui::CloseCurrentPopup();
				}
			}
			else
				ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (open_rename_folder)
	{
		ImGui::OpenPopup("FoldersRenamePopup");

		if (selected_folders.size() > 0)
		{
			int size = selected_folders[0]->folder_name.size();

			if (size > 50)
				size = 50;

			memset(name_tmp, 0, sizeof(char) * 50);

			strcpy_s(name_tmp, sizeof(char) * size + 1, selected_folders[0]->folder_name.c_str());
		}
	}

	if (ImGui::BeginPopup("FoldersRenamePopup"))
	{
		ImGui::Text("Folder Name: ");

		ImGui::SameLine();

		ImGui::InputText("", name_tmp, sizeof(char) * 50, ImGuiInputTextFlags_AutoSelectAll);

		if (ImGui::Button("Accept"))
		{
			if (selected_folders.size() > 0)
			{
				std::string name = name_tmp;

				if (name.size() > 0)
				{
					App->assets->RenameAssetsFolder(selected_folders[0]->dfp.path.c_str(), name_tmp);

					update_folders = true;

					ImGui::CloseCurrentPopup();
				}
			}
			else
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

void ExplorerWindow::DrawFoldersPopupExtern()
{
}

void ExplorerWindow::FoldersDragAndDrop(ExplorerFolder* folder)
{
	// Folder slot become drag target
	uint drag_drop_flags = ImGuiDragDropFlags_SourceNoDisableHover;

	if (App->assets->GetAssetsPath().compare(folder->dfp.path) != 0)
	{
		if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
		{
			int size = sizeof(int);
			int a = 1;
			ImGui::SetDragDropPayload("Folders", &a, size);

			std::vector<ExplorerFolder*> folders = selected_folders;

			for (std::vector<ExplorerFolder*>::iterator it = folders.begin(); it != folders.end(); ++it)
			{
				ImGui::Text((*it)->folder_name.c_str());
			}

			dragging = true;

			ImGui::EndDragDropSource();
		}
	}

	// Folder slot become drop target
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Folders"))
		{
			std::vector<ExplorerFolder*> selected_gos = selected_folders;

			for (std::vector<ExplorerFolder*>::iterator it = selected_folders.begin(); it != selected_folders.end(); ++it)
			{
				bool succes = App->assets->MoveAssetsFolder((*it)->dfp.path.c_str(), folder->dfp.path.c_str());

				if (succes)
				{
					update_files = true;
					update_folders = true;
				}
			}

			dragging = false;
		}

		ImGui::EndDragDropTarget();
	}

	// Files slot become drop target
	if (ImGui::BeginDragDropTarget())
	{
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("Files"))
		{
			std::vector<ExplorerFile*> files = selected_files;

			for (std::vector<ExplorerFile*>::iterator it = files.begin(); it != files.end(); ++it)
			{
				bool succes = App->assets->MoveAsset((*it)->dfp.file_path.c_str(), folder->dfp.path.c_str());

				if(succes)
					update_files = true;
			}

			dragging = false;
		}

		ImGui::EndDragDropTarget();
	}
}

void ExplorerWindow::AddToSelectedFolders(ExplorerFolder * folder)
{
	bool already_selected = false;
	std::vector<ExplorerFolder*> folders = selected_folders;
	for (std::vector<ExplorerFolder*>::iterator it = folders.begin(); it != folders.end(); ++it)
	{
		if (folder->dfp.path.compare((*it)->dfp.path) == 0)
		{
			already_selected = true;
			break;
		}
	}

	if (!already_selected)
	{
		folder->selected = true;
		selected_folders.push_back(folder);
	}
}

void ExplorerWindow::RemoveFromSelectedFolders(ExplorerFolder * folder)
{
	for (std::vector<ExplorerFolder*>::iterator it = selected_folders.begin(); it != selected_folders.end(); ++it)
	{
		if (folder->dfp.path.compare((*it)->dfp.path) == 0)
		{
			folder->selected = false;
			selected_folders.erase(it);
			break;
		}
	}
}

void ExplorerWindow::RemoveAllFromSelectedFolders()
{
	for (std::vector<ExplorerFolder*>::iterator it = selected_folders.begin(); it != selected_folders.end(); ++it)
	{
		(*it)->selected = false;
	}

	selected_folders.clear();
}

void ExplorerWindow::FilesInput(ExplorerFile * file, bool left_clicked, bool right_clicked)
{
	uint selected_files_count = selected_files.size();

	// Input
	if ((App->input->GetKeyRepeat(SDL_SCANCODE_LCTRL) || App->input->GetKeyRepeat(SDL_SCANCODE_RCTRL)) && ImGui::IsItemClicked(0))
	{
		if (!file->selected)
		{
			AddToSelectedFiles(file);
		}
		else
		{
			RemoveFromSelectedFiles(file);
		}

		disable_button_up = true;
	}

	// If shift is pressed do fill gap selection
	else if ((App->input->GetKeyRepeat(SDL_SCANCODE_LSHIFT) || App->input->GetKeyRepeat(SDL_SCANCODE_RSHIFT)) && ImGui::IsItemClicked(0))
	{

	}
	// Monoselection
	else
	{
		if ((left_clicked || right_clicked) && !file->selected)
		{
			RemoveAllFromSelectedFiles();
			AddToSelectedFiles(file);

			App->editor->inspector_window->SetShowingResource(file->resource);

			disable_button_up = true;
		}
		else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP && ImGui::IsItemHovered() && file->selected && selected_files_count == 1)
		{
			if (!disable_button_up)
			{
				RemoveFromSelectedFiles(file);
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
					AddToSelectedFiles(file);
				}
			}
			else
				disable_button_up = false;
		}

		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && !ImGui::IsAnyItemHovered() && ImGui::IsMouseHoveringWindow())
		{
			RemoveAllFromSelectedFiles();
		}
	}
}

void ExplorerWindow::DrawFilesPopupIntern(bool left_clicked, bool right_clicked)
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

			if (selected[0]->resource != nullptr)
			{
				bool close = selected[0]->resource->DrawEditorExplorer();

				if(close)
					ImGui::CloseCurrentPopup();

				ImGui::Separator();
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

		if (ImGui::Button("Delete"))
		{
			for (std::vector<ExplorerFile*>::iterator it = selected.begin(); it != selected.end(); ++it)
			{
				App->assets->UnloadAssetFromEngine((*it)->dfp.file_path.c_str());

				update_files = true;
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

			memset(name_tmp, 0, sizeof(char) * 50);

			strcpy_s(name_tmp, sizeof(char) * size + 1, name.c_str());
		}
	}

	if (ImGui::BeginPopup("RenamePopup"))
	{
		ImGui::Text("Name: ");

		ImGui::SameLine();

		ImGui::InputText("", name_tmp, sizeof(char) * 50, ImGuiInputTextFlags_AutoSelectAll);

		if (ImGui::Button("Accept"))
		{
			if (selected.size() > 0)
			{
				App->assets->RenameAsset(selected[0]->dfp.file_path.c_str(), name_tmp);
				
				update_files = true;
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

void ExplorerWindow::DrawFilesPopupExtern()
{
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN && !ImGui::IsAnyItemHovered() 
		&& ImGui::IsMouseHoveringWindow() && selected_files.size() == 0)
	{
		ImGui::OpenPopup("CreatePopup");
	}

	bool create_script = false;

	if (ImGui::BeginPopup("CreatePopup"))
	{
		if (ImGui::Button("Create Script"))
		{
			create_script = true;

			ImGui::CloseCurrentPopup();
		}

		if (ImGui::Button("Import Asset"))
		{
			bool canceled = false;

			const char* filter[1] = { "" };
			std::string file = App->file_system->SelectFileDilog(canceled, filter);

			if (!canceled)
			{
				App->assets->LoadFileToEngine(file.c_str());
			}

			ImGui::CloseCurrentPopup();

			update_files = true;
			update_folders = true;
		}

		ImGui::EndPopup();
	}

	if (create_script)
	{
		ImGui::OpenPopup("CreateScriptPopup");
		
		memset(name_tmp, 0, sizeof(char) * 50);
	}

	if (ImGui::BeginPopup("CreateScriptPopup"))
	{
		ImGui::Text("Script Name: ");

		ImGui::SameLine();

		ImGui::InputText("", name_tmp, sizeof(char) * 50, ImGuiInputTextFlags_AutoSelectAll);

		if (ImGui::Button("Accept"))
		{			
			std::string name = name_tmp;

			if (name.size() > 0)
			{
				App->assets->CreateScript(App->assets->GetCurrentAssetsPath().c_str(), name_tmp);

				update_files = true;

				ImGui::CloseCurrentPopup();
			}
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}

void ExplorerWindow::FilesDragAndDrop(ExplorerFile * file)
{
	// Folder slot become drag target
	uint drag_drop_flags = ImGuiDragDropFlags_SourceNoDisableHover;
	
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		int size = sizeof(int);
		int a = 1;
		ImGui::SetDragDropPayload("Files", &a, size);

		std::vector<ExplorerFile*> files = selected_files;

		for (std::vector<ExplorerFile*>::iterator it = files.begin(); it != files.end(); ++it)
		{
			ImGui::Text((*it)->dfp.file_name.c_str());
		}

		dragging = true;

		ImGui::EndDragDropSource();
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

ExplorerFolder::ExplorerFolder()
{
}
