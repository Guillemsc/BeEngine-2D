#ifndef __MODULE_EDITOR_H__
#define __MODULE_EDITOR_H__

#include "Module.h"
#include "GeometryMath.h"

class EditorWindow
{
	friend class ModuleEditor;

public:
	EditorWindow();

	virtual void CleanUp() {};
	virtual void DrawEditor() {};

	std::string GetName() const;
	void SetOpened(bool set);
	bool GetOpened() const;

private:
	std::string name;
	bool		opened = true;
};

class ModuleEditor : public Module
{
public:
	ModuleEditor();
	~ModuleEditor();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

	void RenderEditor();
	void EditorInput(SDL_Event event);

	void AddEditorWindow(const char* name, EditorWindow* window);
	void DestroyAllEditorWindows();
	void DrawEditorWindows();

private:
	void ImGuiInit();
	void ImGuiNewFrame();
	void ImGuiQuit();

	void MenuBar();
	void ToolsBar(float2 margins_left_up);
	void DockingSpace(float2 margins_left_up, float2 margins_right_down);

	void LoadCustomStyle();

private:
	std::vector<EditorWindow*> editor_windows;
};

#endif // !__MODULE_EDITOR_H__