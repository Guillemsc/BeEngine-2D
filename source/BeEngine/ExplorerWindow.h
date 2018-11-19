#ifndef __EXPLORER_WINDOW_H__
#define __EXPLORER_WINDOW_H__

#include "ModuleEditor.h"
#include "imgui.h"
#include "ModuleFileSystem.h"

class GameObject;
class Event;

class ExplorerFile
{
public:
	ExplorerFile();

	DecomposedFilePath dfp;

	bool selected = false;
};

class ExplorerWindow : public EditorWindow
{
	friend class ModuleEditor;

private:
	void operator delete(void *) {}

public:
	ExplorerWindow();
	~ExplorerWindow();

	void OnEvent(Event* ev);

	void Start();
	void CleanUp();
	void DrawEditor();
	ImGuiWindowFlags GetWindowFlags();

private:
	void UpdateFoldersAndFiles();
	void ClearFloldersAndFiles();

	void DrawFoldersColumn();
	void DrawFilesColumn();

	// Folders Column
	void DrawFoldersRecursive(const Folder& folder);
	void SetSelectedFolderTree(const char* path);

	void FoldersInput(const std::string& folder, bool left_clicked, bool right_clicked);

	// Files column
	void FilesInput(ExplorerFile* file, bool left_clicked, bool right_clicked);
	void DrawFilesPopup(bool left_clicked, bool right_clicked);

	void AddToSelectedFiles(ExplorerFile* add);
	void RemoveFromSelectedFiles(ExplorerFile* add);
	void RemoveAllFromSelectedFiles();
	std::vector<ExplorerFile*> GetSelectedFiles();

private:
	ImFont* font = nullptr;
	
	Folder folder_tree;
	std::vector<ExplorerFile*> cur_files;

	std::vector<ExplorerFile*> selected_files;

	bool update_folders = false;

	bool disable_button_up = true;
	bool dragging = false;

	char change_name_tmp[50];
};

#endif // !__HIERARCHY_WINDOW_H__