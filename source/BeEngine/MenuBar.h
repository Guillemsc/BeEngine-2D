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

	void CleanUp();
	void DrawEditor();

private:

};

#endif // !__MENU_BAR_H__