#ifndef __PROFILER_WINDOW_H__
#define __PROFILER_WINDOW_H__

#include "ModuleEditor.h"
#include "imgui.h"

class Profile;

class ProfilerWindow : public EditorWindow
{
	friend class ModuleEditor;

private:
	void operator delete(void *) {}

public:
	ProfilerWindow();
	~ProfilerWindow();

	void Start();
	void CleanUp();
	void DrawEditor();
	ImGuiWindowFlags GetWindowFlags();

private: 
	void DrawProfileRecursive(Profile* prof, int &draw_counter, bool profiles_update);

private:
	ImFont* font = nullptr;
};

#endif // !__PROFILER_WINDOW_H__