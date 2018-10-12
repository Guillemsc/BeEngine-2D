#ifndef __DOCKING_SPACE_H__
#define __DOCKING_SPACE_H__

#include "ModuleEditor.h"

class DockingSpace : public EditorElement
{
	friend class ModuleEditor;

private:
	void operator delete(void *) {}

public:
	DockingSpace(float2 margins_left_up, float2 margins_right_down);
	~DockingSpace();

	void Start();
	void CleanUp();
	void DrawEditor();

private:
	float2 margins_left_up;
	float2 margins_right_down;

	ImFont * font = nullptr;

	bool opened = true;
};

#endif // !__MENU_BAR_H__