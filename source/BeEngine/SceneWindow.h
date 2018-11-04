#ifndef __SCENE_WINDOW_H__
#define __SCENE_WINDOW_H__

#include "ModuleEditor.h"
#include "imgui.h"

class SceneWindow : public EditorWindow
{
	friend class ModuleEditor;

private:
	void operator delete(void *) {}

public:
	SceneWindow();
	~SceneWindow();

	void Start();
	void CleanUp();
	void DrawEditor();
	ImGuiWindowFlags GetWindowFlags();

	Rect GetSceneRect() const;

private:
	float2 last_size = float2::zero;

	ImFont* font = nullptr;

	Rect scene_viewport_rect;
};

#endif // !__SCENE_WINDOW_H__