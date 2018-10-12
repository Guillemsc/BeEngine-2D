#ifndef __GAME_WINDOW_H__
#define __GAME_WINDOW_H__

#include "ModuleEditor.h"

class GameWindow : public EditorWindow
{
	friend class ModuleEditor;

private:
	void operator delete(void *) {}

public:
	GameWindow();
	~GameWindow();

	void CleanUp();
	void DrawEditor();

private:

};

#endif // !__GAME_WINDOW_H__