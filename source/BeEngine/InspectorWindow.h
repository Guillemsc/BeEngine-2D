#ifndef __INSPECTOR_WINDOW_H__
#define __INSPECTOR_WINDOW_H__

#include "ModuleEditor.h"
#include "imgui.h"

class GameObject;

class InspectorWindow : public EditorWindow
{
	friend class ModuleEditor;

private:
	void operator delete(void *) {}

public:
	InspectorWindow();
	~InspectorWindow();

	void Start();
	void CleanUp();
	void DrawEditor();
	ImGuiWindowFlags GetWindowFlags();

private:


private:
	ImFont* font = nullptr;
};

#endif // !__INSPECTOR_WINDOW_H__