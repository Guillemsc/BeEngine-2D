#include "ModuleEditor.h"
#include "App.h"
#include "GeometryMath.h"
#include "ModuleWindow.h"
#include "ModuleRenderer.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "MenuBar.h"
#include "ToolsBar.h"
#include "DockingSpace.h"
#include "SceneWindow.h"
#include "ProjectManager.h"
#include "ProgressWindow.h"
#include "ProfilerWindow.h"
#include "ModuleInput.h"
#include "ResourcesWindow.h"
#include "HierarchyWindow.h"
#include "InspectorWindow.h"
#include "imgui_docking.h"

ModuleEditor::ModuleEditor()
{
	docking_layouts_json_filepath = App->GetConfigurationPath() + std::string("layouts.json");
}

ModuleEditor::~ModuleEditor()
{
}

void ModuleEditor::CreateProfiles()
{
	prof_editor_elements_draw = prof_module_preupdate->AddProfileChild("Editor elements draw");
	prof_editor_windows_draw = prof_module_preupdate->AddProfileChild("Editor windows draw");
}

bool ModuleEditor::Awake()
{
	bool ret = true;

	ImGuiInit();

	docking_space = (DockingSpace*)AddEditorElement(new DockingSpace(float2(0, 58), float2(0, 0)));
	menu_bar = (MenuBar*)AddEditorElement(new MenuBar(), true);
	tools_bar = (ToolsBar*)AddEditorElement(new ToolsBar(float2(0, 19)), true);
	project_manager = (ProjectManager*)AddEditorElement(new ProjectManager(), true);
	progress_window = (ProgressWindow*)AddEditorElement(new ProgressWindow(), true);

	scene_window = (SceneWindow*)AddEditorWindow("Scene", new SceneWindow());
	AddEditorWindow("Profiler", new ProfilerWindow());
	AddEditorWindow("Resources", new ResourcesWindow());
	AddEditorWindow("Hierarchy", new HierarchyWindow());
	inspector_window = (InspectorWindow*)AddEditorWindow("Inspector", new InspectorWindow());

	LoadDockingProfiles();

	return ret;
}

bool ModuleEditor::Start()
{
	bool ret = true;

	SetEditorState(EditorState::PROJECT_MANAGER);

	return ret;
}

bool ModuleEditor::PreUpdate()
{
	bool ret = true;

	ImGuiStartFrame();

	DrawEditorElements();
	DrawEditorWindows();

	ImGuiEndFrame();

	return ret;
}

bool ModuleEditor::Update()
{
	bool ret = true;

	//ImGui::ShowDemoWindow(&demo_window_open);
	
	return ret;
}

bool ModuleEditor::PostUpdate()
{
	bool ret = true;

	ImGuiEndFrame();

	return ret;
}

bool ModuleEditor::CleanUp()
{
	bool ret = true;

	SaveCurrentDockingProfile();

	DestroyAllEditorWindows();
	DestroyAllEditorElements();

	editor_fonts.clear();

	ImGuiQuit();

	return ret;
}

void ModuleEditor::RenderEditor()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ModuleEditor::EditorInput(SDL_Event event)
{
	ImGui_ImplSDL2_ProcessEvent(&event);
}

EditorElement* ModuleEditor::AddEditorElement(EditorElement* element, bool visible)
{
	EditorElement* ret = nullptr;

	if (element != nullptr)
	{
		bool exists = false;
		for (std::vector<EditorElement*>::iterator it = editor_elements.begin(); it != editor_elements.end(); ++it)
		{
			if ((*it) == element)
			{
				exists = true;
				break;
			}
		}

		if (!exists)
		{
			editor_elements.push_back(element);

			ret = element;
			ret->SetVisible(visible);

			element->Start();
		}
	}

	return ret;
}

void ModuleEditor::DestroyAllEditorElements()
{
	for (std::vector<EditorElement*>::iterator it = editor_elements.begin(); it != editor_elements.end(); ++it)
	{
		(*it)->CleanUp();
		RELEASE(*it);
	}

	editor_elements.clear();
}

void ModuleEditor::DrawEditorElements()
{
	prof_editor_elements_draw->Start();

	for (std::vector<EditorElement*>::iterator it = editor_elements.begin(); it != editor_elements.end(); ++it)
	{
		if ((*it)->GetVisible())
			(*it)->DrawEditor();
	}

	prof_editor_elements_draw->Finish();
}

EditorWindow* ModuleEditor::AddEditorWindow(const char * name, EditorWindow * window)
{
	EditorWindow* ret = nullptr;;

	if (window != nullptr)
	{
		bool exists = false;

		for (std::vector<EditorWindow*>::iterator it = editor_windows.begin(); it != editor_windows.end(); ++it)
		{
			if ((*it)->GetName().compare(name) == 0)
			{
				exists = true;
				break;
			}
		}

		if (!exists)
		{
			window->name = name;
			window->prof_draw = prof_editor_windows_draw->AddProfileChild(window->name.c_str());
			editor_windows.push_back(window);

			window->Start();

			ret = window;
		}
	}

	return ret;
}

void ModuleEditor::DestroyAllEditorWindows()
{
	for (std::vector<EditorWindow*>::iterator it = editor_windows.begin(); it != editor_windows.end(); ++it)
	{
		(*it)->CleanUp();
		RELEASE(*it);
	}

	editor_windows.clear();
}

void ModuleEditor::DrawEditorWindows()
{
	prof_editor_windows_draw->Start();
	
	for (std::vector<EditorWindow*>::iterator it = editor_windows.begin(); it != editor_windows.end(); ++it)
	{
		(*it)->prof_draw->Start();

		ImGuiWindowFlags flags = 0;
		flags |= (*it)->GetWindowFlags();

		if (ImGui::BeginDock((*it)->name.c_str(), &(*it)->opened, flags))
		{
			ImVec2 win_pos = ImGui::GetWindowPos();
			ImVec2 win_size = ImGui::GetWindowSize();

			(*it)->window_pos = float2(win_pos.x, win_pos.y);
			(*it)->window_size = float2(win_size.x, win_size.y);

			if(draw_editor)
				(*it)->DrawEditor();
		}

		ImGui::EndDock();

		(*it)->prof_draw->Finish();
	}
	

	prof_editor_windows_draw->Finish();
}

void ModuleEditor::SetEditorState(const EditorState & state)
{
	switch (state)
	{
	case EditorState::PROJECT_MANAGER:
		project_manager->SetVisible(true);
		docking_space->SetVisible(false);
		menu_bar->SetVisible(false);
		tools_bar->SetVisible(false);

		for (std::vector<EditorWindow*>::iterator it = editor_windows.begin(); it != editor_windows.end(); ++it)
			(*it)->SetVisible(false);
		
		break;
	case EditorState::MAIN_ENGINE:
		project_manager->SetVisible(false);
		docking_space->SetVisible(true);
		menu_bar->SetVisible(true);
		tools_bar->SetVisible(true);

		for (std::vector<EditorWindow*>::iterator it = editor_windows.begin(); it != editor_windows.end(); ++it)
			(*it)->SetVisible(true);
		break;
	}

	editor_state = state;
}

ImFont* ModuleEditor::GetLoadedFont(const char * name)
{
	ImFont* ret = nullptr;

	ret = editor_fonts[name];

	return ret;
}

void ModuleEditor::SetDrawEditor(bool set)
{
	draw_editor = set;
}

void ModuleEditor::ImGuiInit()
{
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui_ImplSDL2_InitForOpenGL(App->window->GetWindow(), App->renderer->GetSDLGLContext());

	const char* glsl_version = "#version 130";
	ImGui_ImplOpenGL3_Init(glsl_version);

	LoadCustomStyle();

	LoadImGuiFont("Roboto-Medium.ttf", 16, "RobotoMedium_16");
	LoadImGuiFont("Roboto-Medium.ttf", 18, "RobotoMedium_18");
	LoadImGuiFont("Roboto-Medium.ttf", 30, "RobotoMedium_30");
	LoadImGuiFont("Roboto-Medium.ttf", 60, "RobotoMedium_60");
	LoadImGuiFont("Roboto-Medium.ttf", 15, "RobotoMedium_15");
	LoadImGuiFont("Roboto-Black.ttf", 16, "RobotoBlack_16");
	LoadImGuiFont("Roboto-Bold.ttf", 17, "RobotoBold_17");
	LoadImGuiFont("Roboto-MediumItalic.ttf", 16, "RobotoMediumItalic_16");
	LoadImGuiFont("Roboto-Light.ttf", 16, "RobotoLight_16");
	LoadImGuiFont("RobotoCondensed-Regular.ttf", 16, "RobotoCondensed_16");
	LoadImGuiFont("FontAwesome.ttf", 16, "Awesome_16");
}

void ModuleEditor::ImGuiStartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->GetWindow());
	ImGui::NewFrame();
}

void ModuleEditor::ImGuiEndFrame()
{
}

void ModuleEditor::ImGuiQuit()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

ImFont* ModuleEditor::LoadImGuiFont(const char * filename, int size, const char * load_name)
{
	ImFont* ret = nullptr;

	ImGuiIO& io = ImGui::GetIO();
	
	std::string font_path = std::string("fonts\\") + filename;

	ret = io.Fonts->AddFontFromFileTTF(font_path.c_str(), size);

	editor_fonts[load_name] = ret;

	return ret;
}

void ModuleEditor::LoadCustomStyle()
{
	ImGuiStyle* style = &ImGui::GetStyle();

	ImVec4* colors = ImGui::GetStyle().Colors;

	style->PopupRounding = 7;
	style->IndentSpacing = 18;
	style->ChildRounding = 0;
	style->FrameRounding = 3;
	style->WindowRounding = 3.0f;

	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.12f, 0.12f, 0.12f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.28f, 0.28f, 0.28f, 0.65f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.65f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.52f, 0.52f, 0.52f, 0.65f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.90f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.45f, 0.76f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.07f, 0.07f, 0.06f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.49f, 0.78f, 0.98f, 0.65f);
	colors[ImGuiCol_Button] = ImVec4(0.00f, 0.51f, 0.87f, 0.65f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.49f, 0.78f, 0.98f, 0.65f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.49f, 0.78f, 0.98f, 0.65f);
	colors[ImGuiCol_Header] = ImVec4(0.00f, 0.51f, 0.87f, 0.65f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.50f, 0.78f, 0.98f, 0.65f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 0.51f, 0.87f, 0.65f);
	colors[ImGuiCol_Separator] = ImVec4(0.29f, 0.29f, 0.29f, 0.65f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.00f, 0.51f, 0.87f, 0.65f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.00f, 0.51f, 0.87f, 0.65f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.51f, 0.87f, 0.65f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.49f, 0.78f, 0.98f, 0.65f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 0.51f, 0.87f, 0.65f);
	colors[ImGuiCol_Tab] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.28f, 0.28f, 0.28f, 0.98f);
	colors[ImGuiCol_TabActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.49f, 0.78f, 0.98f, 0.65f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.00f, 0.51f, 0.87f, 0.65f);
	colors[ImGuiCol_DockingPreview] = ImVec4(0.00f, 0.51f, 0.87f, 0.65f);
	colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.13f, 0.13f, 0.13f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 0.46f, 0.61f, 0.65f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.49f, 0.78f, 0.98f, 0.65f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void ModuleEditor::LoadDockingProfiles()
{
	JSON_Doc* doc = App->json->LoadJSON(docking_layouts_json_filepath.c_str());

	if (doc == nullptr)
		doc = App->json->CreateJSON(docking_layouts_json_filepath.c_str());

	if (doc != nullptr)
	{
		std::string curr_layout = doc->GetString("curr_layout");

		if (!doc->ArrayExists("layouts"))
			doc->SetArray("layouts");

		int layouts_arr_count = doc->GetArrayCount("layouts");

		std::map<int, std::string> layouts_to_load;

		for (int i = 0; i < layouts_arr_count; ++i)
		{
			std::string layout_name = doc->GetStringFromArray("layouts", i);

			layouts_to_load[i] = layout_name;
		}

		for (std::map<int, std::string>::iterator it = layouts_to_load.begin(); it != layouts_to_load.end(); ++it)
		{
			if (!ImGui::LayoutExists(doc, (*it).second.c_str()) || DockingProfileExists((*it).second.c_str()))
			{
				doc->RemoveArrayIndex("layouts", (*it).first);
			}
			else
			{
				docking_profiles.push_back((*it).second);
			}
		}

		doc->Save();

		SetCurrentDockingProfile(curr_layout.c_str());
	}
}

bool ModuleEditor::SetCurrentDockingProfile(const char * set, bool load)
{
	bool ret = false;

	JSON_Doc* doc = App->json->LoadJSON(docking_layouts_json_filepath.c_str());

	if (doc != nullptr)
	{
		if (DockingProfileExists(set))
		{
			if(load)
				ImGui::LoadLayout(doc, set);

			current_docking_profile = set;

			ret = true;
		}
		else
		{
			current_docking_profile = "default";
			CreateNewDockingProfile(current_docking_profile.c_str());
		}

		doc->SetString("curr_layout", current_docking_profile.c_str());

		doc->Save();
	}

	return ret;
}

const char * ModuleEditor::GetCurrentDockingProfile() const
{
	return current_docking_profile.c_str();
}

bool ModuleEditor::CreateNewDockingProfile(const char * name)
{
	bool ret = false;

	if (!DockingProfileExists(name))
	{
		JSON_Doc* doc = App->json->LoadJSON(docking_layouts_json_filepath.c_str());

		if (doc != nullptr)
		{
			doc->AddStringToArray("layouts", name);

			ImGui::SaveLayout(doc, name);

			docking_profiles.push_back(name);

			doc->Save();

			ret = true;
		}
	}

	return ret;
}

bool ModuleEditor::CanRemoveDockingProfile(const char * name)
{
	bool ret = true;

	std::string default = "default";

	if (default.compare(name) == 0)
		ret = false;

	return ret;
}

bool ModuleEditor::RemoveDockingProfile(const char * name)
{
	bool ret = false;

	if (CanRemoveDockingProfile(name))
	{
		if (DockingProfileExists(name))
		{
			for (std::vector<std::string>::iterator it = docking_profiles.begin(); it != docking_profiles.end(); ++it)
			{
				if ((*it).compare(name) == 0)
				{
					docking_profiles.erase(it);
					break;
				}
			}

			JSON_Doc* doc = App->json->LoadJSON(docking_layouts_json_filepath.c_str());

			if (doc != nullptr)
			{
				int layouts_arr_count = doc->GetArrayCount("layouts");

				for (int i = 0; i < layouts_arr_count; ++i)
				{
					std::string layout_name = doc->GetStringFromArray("layouts", i);

					if (layout_name.compare(current_docking_profile.c_str()) == 0)
					{
						doc->RemoveArrayIndex("layouts", i);
						break;
					}
				}

				ImGui::RemoveLayout(doc, current_docking_profile.c_str());

				doc->Save();
			}

			SetCurrentDockingProfile("default", false);

			ret = true;
		}
	}

	return ret;
}

bool ModuleEditor::RemoveCurrentDockingProfile()
{
	bool ret = false;

	ret = RemoveDockingProfile(current_docking_profile.c_str());

	return ret;
}

std::vector<std::string> ModuleEditor::GetDockingProfiles() const
{
	return docking_profiles;
}

std::vector<EditorWindow*> ModuleEditor::GetEditorWindows() const
{
	return editor_windows;
}

bool ModuleEditor::SaveCurrentDockingProfile()
{
	bool ret = false;

	if (DockingProfileExists(current_docking_profile.c_str()))
	{
		JSON_Doc* doc = App->json->LoadJSON(docking_layouts_json_filepath.c_str());

		int layouts_arr_count = doc->GetArrayCount("layouts");

		for (int i = 0; i < layouts_arr_count; ++i)
		{
			std::string layout_name = doc->GetStringFromArray("layouts", i);

			if (layout_name.compare(current_docking_profile.c_str()) == 0)
			{
				ImGui::SaveLayout(doc, current_docking_profile.c_str());
				break;
			}
		}

		doc->Save();
	}

	return ret;
}

bool ModuleEditor::DockingProfileExists(const char * name)
{
	bool ret = false;

	for (std::vector<std::string>::iterator it = docking_profiles.begin(); it != docking_profiles.end(); ++it)
	{
		if ((*it).compare(name) == 0)
		{
			ret = true;
			break;
		}
	}

	return ret;
}

EditorWindow::EditorWindow()
{
}

std::string EditorWindow::GetName() const
{
	return name;
}

void EditorWindow::SetVisible(bool set)
{
	visible = set;
}

bool EditorWindow::GetVisible() const
{
	return visible;
}

bool EditorWindow::GetOpened() const
{
	return opened;
}

float2 EditorWindow::GetWindowSize() const
{
	return window_size;
}

float2 EditorWindow::GetWindowPos() const
{
	return window_pos;
}

Rect EditorWindow::GetWindowRect()
{
	return Rect(window_pos.x, window_pos.y, window_size.x, window_size.y);
}

bool EditorWindow::GetMouseInsideWindow() const
{
	bool ret = false;
	
	float2 mouse_pos = App->input->GetMouse();

	if (mouse_pos.x > window_pos.x && mouse_pos.x < window_pos.x + window_size.x)
	{
		if (mouse_pos.y > window_pos.y && mouse_pos.y < window_pos.y + window_size.y)
		{
			ret = true;
		}
	}

	return ret;
}

EditorElement::EditorElement()
{
}

void EditorElement::SetVisible(bool set)
{
	visible = set;
}

bool EditorElement::GetVisible() const
{
	return visible;
}
