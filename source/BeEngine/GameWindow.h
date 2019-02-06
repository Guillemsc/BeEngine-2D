#ifndef __GAME_WINDOW_H__
#define __GAME_WINDOW_H__

#include "ModuleEditor.h"
#include "imgui.h"

class Camera2D;

class GameWindow : public EditorWindow
{
	friend class ModuleEditor;

private:
	void operator delete(void *) {}

public:
	GameWindow();
	~GameWindow();

	void Start();
	void CleanUp();
	void DrawEditor();
	ImGuiWindowFlags GetWindowFlags();

	Rect GetGameRect() const;

private:
	float2 last_size = float2::zero;

	ImFont* font = nullptr;

	Rect game_viewport_rect;

	Camera2D* last_game_camera = nullptr;

	bool use_reference_size = false;
	float2 reference_size = float2::zero;
};

#endif // !__SCENE_WINDOW_H__