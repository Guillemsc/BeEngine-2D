#ifndef __MENU_BAR_H__
#define __MENU_BAR_H__

#include "ModuleEditor.h"

class MenuBar : public EditorElement
{
	friend class ModuleEditor;

private:
	void operator delete(void *) {}

public:
	MenuBar();
	~MenuBar();

	void Start();
	void CleanUp();
	void DrawEditor();

private:
	void DrawLayoutsEditor();
	void DrawWindowsEditor();

private:
	ImFont* font = nullptr;

	char new_layout_name[50];
};

#endif // !__MENU_BAR_H__