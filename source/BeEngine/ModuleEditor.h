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
class ProgressWindow;
class SceneWindow;
class ExplorerWindow;
class InspectorWindow;
class ConsoleWindow;

class EditorWindow
{
	friend class ModuleEditor;
	friend class MenuBar;

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
	float2 GetWindowPos() const;
	Rect GetWindowRect();
	bool GetMouseInsideWindow() const;

private:
	std::string name;
	bool		opened = true;
	bool		visible = true;
	float2		window_pos = float2::zero;
	float2	    window_size = float2::zero;

	Profile* prof_draw = nullptr;
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

	void CreateProfiles();
	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

	void RenderEditor();
	void EditorInput(SDL_Event event);

	void SetEditorState(const EditorState& state);

	ImFont* GetLoadedFont(const char* name);

	void SetDrawEditor(bool set);

	bool SetCurrentDockingProfile(const char* set, bool load = true);
	bool SaveCurrentDockingProfile();
	const char* GetCurrentDockingProfile() const;
	bool CreateNewDockingProfile(const char* name);
	bool CanRemoveDockingProfile(const char* name);
	bool RemoveDockingProfile(const char* name);
	bool RemoveCurrentDockingProfile();
	std::vector<std::string> GetDockingProfiles() const;

	std::vector<EditorWindow*> GetEditorWindows() const;

private:
	EditorElement* AddEditorElement(EditorElement* element, bool visible = true);
	void DestroyAllEditorElements();
	void DrawEditorElements();

	EditorWindow* AddEditorWindow(const char* name, EditorWindow* window);
	void DestroyAllEditorWindows();
	void DrawEditorWindows();

	void ImGuiInit();
	void ImGuiStartFrame();
	void ImGuiEndFrame();
	void ImGuiQuit();

	ImFont* LoadImGuiFont(const char* filename, int size, const char* load_name);

	void LoadCustomStyle();

	void LoadDockingProfiles();
	bool DockingProfileExists(const char* name);

public:
	DockingSpace*    docking_space = nullptr;
	MenuBar*         menu_bar = nullptr;
	ToolsBar*        tools_bar = nullptr;
	ProjectManager*  project_manager = nullptr;
	ProgressWindow*  progress_window = nullptr;

	SceneWindow*     scene_window = nullptr;
	InspectorWindow* inspector_window = nullptr;
	ExplorerWindow*  explorer_window = nullptr;
	ConsoleWindow*	 console_window = nullptr;

private:
	EditorState editor_state;

	std::vector<EditorElement*> editor_elements;
	std::vector<EditorWindow*> editor_windows;

	std::map<std::string, ImFont*> editor_fonts;

	std::string docking_layouts_json_filepath;

	bool load_docking_profile = true;
	std::vector<std::string> docking_profiles;
	std::string current_docking_profile;

	Profile* prof_editor_windows_draw = nullptr;
	Profile* prof_editor_elements_draw = nullptr;

	bool draw_editor = true;

	bool demo_window_open = true;
};

#endif // !__MODULE_EDITOR_H__