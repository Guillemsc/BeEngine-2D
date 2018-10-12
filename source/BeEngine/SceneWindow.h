#ifndef __GAME_WINDOW_H__
#define __GAME_WINDOW_H__

#include "ModuleEditor.h"

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

private:
	float2 last_size = float2::zero;
};

#endif // !__GAME_WINDOW_H__