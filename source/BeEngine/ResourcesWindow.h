#ifndef __RESOURCES_WINDOW_H__
#define __RESOURCES_WINDOW_H__

#include "ModuleEditor.h"
#include "imgui.h"

class Profile;

class ResourcesWindow : public EditorWindow
{
	friend class ModuleEditor;

private:
	void operator delete(void *) {}

public:
	ResourcesWindow();
	~ResourcesWindow();

	void Start();
	void CleanUp();
	void DrawEditor();
	ImGuiWindowFlags GetWindowFlags();

private:
	ImFont * font = nullptr;
};

#endif // !__PROFILER_WINDOW_H__