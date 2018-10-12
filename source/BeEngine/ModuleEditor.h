#ifndef __MODULE_EDITOR_H__
#define __MODULE_EDITOR_H__

#include "Module.h"
#include "GeometryMath.h"

class MenuBar;
class ToolsBar;
struct ImFont;

class EditorWindow
{
	friend class ModuleEditor;

protected:
	void operator delete(void *) {}

public:
	EditorWindow();

	virtual void CleanUp() {};
	virtual void DrawEditor() {};

	std::string GetName() const;
	void SetOpened(bool set);
	bool GetOpened() const;
	float2 GetWindowSize() const;

private:
	std::string name;
	bool		opened = true;
	float2	    window_size = float2::zero;
};

class EditorElement
{
	friend class ModuleEditor;

private:
	void operator delete(void *) {}

public:
	EditorElement();

	virtual void CleanUp() {};
	virtual void DrawEditor() {};

	void SetVisible(bool set);
	bool GetVisible() const;

private:
	bool visible = false;
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

	EditorElement* AddEditorElement(EditorElement* element, bool visible = false);
	void DestroyAllEditorElements();
	void DrawEditorElements();

	void AddEditorWindow(const char* name, EditorWindow* window);
	void DestroyAllEditorWindows();
	void DrawEditorWindows();

private:
	void ImGuiInit();
	void ImGuiStartFrame();
	void ImGuiEndFrame();
	void ImGuiQuit();

	void DockingSpace(float2 margins_left_up, float2 margins_right_down);

	void LoadCustomStyle();

public:
	MenuBar* menu_bar = nullptr;
	ToolsBar* tools_bar = nullptr;

private:
	std::vector<EditorElement*> editor_elements;
	std::vector<EditorWindow*> editor_windows;

	ImFont* font = nullptr;
};

#endif // !__MODULE_EDITOR_H__