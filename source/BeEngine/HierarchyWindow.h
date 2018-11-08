#ifndef __HIERARCHY_WINDOW_H__
#define __HIERARCHY_WINDOW_H__

#include "ModuleEditor.h"
#include "imgui.h"

class GameObject;

class HierarchyWindow : public EditorWindow
{
	friend class ModuleEditor;

private:
	void operator delete(void *) {}

public:
	HierarchyWindow();
	~HierarchyWindow();

	void Start();
	void CleanUp();
	void DrawEditor();
	ImGuiWindowFlags GetWindowFlags();

private:
	void DrawMenuBar();
	void GameObjectInput(GameObject* go);
	void DrawGameObjectRecursive(GameObject* go, bool is_root = false);

private:
	ImFont* font = nullptr;

	GameObject* go_to_drag = nullptr;
};

#endif // !__HIERARCHY_WINDOW_H__