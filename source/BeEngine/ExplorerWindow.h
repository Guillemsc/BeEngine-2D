#ifndef __EXPLORER_WINDOW_H__
#define __EXPLORER_WINDOW_H__

#include "ModuleEditor.h"
#include "imgui.h"
#include "ModuleFileSystem.h"

class GameObject;
class Event;

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
	void DrawFoldersColumn();
	void DrawFilesColumn();

	void DrawFoldersRecursive(const Folder& folder);
	void SetSelectedFolderTree(const char* path);

private:
	ImFont* font = nullptr;
	
	Folder folder_tree;
	std::vector<std::string> selected_folder_files_paths;

	bool update_folders = false;

	bool disable_button_up = true;
	bool dragging = false;

	char change_name_tmp[50];
};

#endif // !__HIERARCHY_WINDOW_H__