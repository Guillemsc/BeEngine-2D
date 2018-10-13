#ifndef __MODULE_EDITOR_H__
#define __MODULE_EDITOR_H__

#include "Module.h"
#include "GeometryMath.h"
#include "imgui.h"
#include <map>

struct ImFont;
class MenuBar;
class ToolsBar;
class DockingSpace;
class ProjectManager;

class EditorWindow
{
	friend class ModuleEditor;

protected:
	void operator delete(void *) {}

public:
	EditorWindow();

	virtual void Start() {};
	virtual void CleanUp() {};
	virtual void DrawEditor() {};
	virtual ImGuiWindowFlags GetWindowFlags() { ImGuiWindowFlags ret = 0; return ret; }

	std::string GetName() const;
	void SetVisible(bool set);
	bool GetVisible() const;
	bool GetOpened() const;
	float2 GetWindowSize() const;

private:
	std::string name;
	bool		opened = true;
	bool		visible = true;
	float2		window_pos = float2::zero;
	float2	    window_size = float2::zero;
};

class EditorElement
{
	friend class ModuleEditor;

private:
	void operator delete(void *) {}

public:
	EditorElement();

	virtual void Start() {};
	virtual void CleanUp() {};
	virtual void DrawEditor() {};

	void SetVisible(bool set);
	bool GetVisible() const;

private:
	bool visible = false;
};

enum EditorState
{
	PROJECT_MANAGER,
	MAIN_ENGINE,
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

	EditorElement* AddEditorElement(EditorElement* element, bool visible = true);
	void DestroyAllEditorElements();
	void DrawEditorElements();

	void AddEditorWindow(const char* name, EditorWindow* window);
	void DestroyAllEditorWindows();
	void DrawEditorWindows();

	void SetEditorState(const EditorState& state);

	ImFont* GetLoadedFont(const char* name);

private:
	void ImGuiInit();
	void ImGuiStartFrame();
	void ImGuiEndFrame();
	void ImGuiQuit();

	ImFont* LoadImGuiFont(const char* filename, int size, const char* load_name);

	void LoadCustomStyle();

public:
	DockingSpace*   docking_space = nullptr;
	MenuBar*        menu_bar = nullptr;
	ToolsBar*       tools_bar = nullptr;
	ProjectManager* project_manager = nullptr;

private:
	EditorState editor_state;

	std::vector<EditorElement*> editor_elements;
	std::vector<EditorWindow*> editor_windows;

	std::map<std::string, ImFont*> editor_fonts;

	bool demo_window_open = true;
};

#endif // !__MODULE_EDITOR_H__