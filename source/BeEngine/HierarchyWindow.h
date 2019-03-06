#ifndef __HIERARCHY_WINDOW_H__
#define __HIERARCHY_WINDOW_H__

#include "ModuleEditor.h"
#include "imgui.h"

class GameObject;
class Scene;

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

	void DrawScenesPopup(bool left_clicked, bool right_clicked, uint scenes_count);
	void SceneInput(Scene* scene, bool left_clicked, bool right_clicked);

	void DrawScene(Scene* scene, uint scene_count, uint& go_count, uint& height_count);

	void DrawGameObjectsPopup(bool left_clicked, bool right_clicked);
	void GameObjectInput(GameObject* go, bool left_clicked, bool right_clicked);

	void DrawGameObjectRecursive(Scene* scene, GameObject* go, uint child_index, uint &go_count, uint& height_count);
	void DragAndDropBeforeChilds(Scene* scene, GameObject* go, uint child_index, uint height_count);
	void DragAndDropAfterChilds(Scene* scene, GameObject* go, uint child_index, uint height_count);

private:
	ImFont* font = nullptr;
	ImFont* scenes_font = nullptr;

	bool disable_button_up = true;
	bool dragging = false;

	char change_name_tmp[50];
};

#endif // !__HIERARCHY_WINDOW_H__