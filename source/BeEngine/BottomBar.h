#ifndef __BOTTOM_BAR_H__
#define __BOTTOM_BAR_H__

#include "ModuleEditor.h"

class BottomBar : public EditorElement
{
	friend class ModuleEditor;

private:
	void operator delete(void *) {}

public:
	BottomBar(float height);
	~BottomBar();

	void Start();
	void CleanUp();
	void DrawEditor();

private:
	ImFont* font = nullptr;

	bool opened = true;

	float height = 0.0f;

};

#endif // !__BOTTOM_BAR_H__