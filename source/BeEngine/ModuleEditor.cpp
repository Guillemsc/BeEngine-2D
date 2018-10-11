#include "ModuleEditor.h"
#include "App.h"
#include "ModuleWindow.h"
#include "MenuBar.h"
#include "ModuleRenderer3D.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"

ModuleEditor::ModuleEditor()
{
}

ModuleEditor::~ModuleEditor()
{
}

bool ModuleEditor::Awake()
{
	bool ret = true;

	ImGuiInit();

	menu_bar = (MenuBar*)AddEditorElement(new MenuBar(), true);

	AddEditorWindow("test", new EditorWindow());

	return ret;
}

bool ModuleEditor::Start()
{
	bool ret = true;

	return ret;
}

bool ModuleEditor::PreUpdate()
{
	bool ret = true;

	ImGuiStartFrame();

	MenuBar();

	ToolsBar(float2(0, 19));

	DockingSpace(float2(0, 50), float2(0, 0));

	DrawEditorElements();
	DrawEditorWindows();

	ImGuiEndFrame();

	return ret;
}

bool ModuleEditor::Update()
{
	bool ret = true;

	bool open = true;

	ImGui::ShowDemoWindow(&open);
	ImGui::Begin("asd", &open);
	ImGui::End();
	
	return ret;
}

bool ModuleEditor::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool ModuleEditor::CleanUp()
{
	bool ret = true;

	DestroyAllEditorWindows();
	DestroyAllEditorElements();

	ImGuiQuit();

	return ret;
}

void ModuleEditor::RenderEditor()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//ImGuiIO& io = ImGui::GetIO(); (void)io;
	//if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	//{
	//	ImGui::UpdatePlatformWindows();
	//	ImGui::RenderPlatformWindowsDefault();
	//}
}

void ModuleEditor::EditorInput(SDL_Event event)
{
	ImGui_ImplSDL2_ProcessEvent(&event);
}

EditorElement* ModuleEditor::AddEditorElement(EditorElement * element, bool visible)
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
	for (std::vector<EditorElement*>::iterator it = editor_elements.begin(); it != editor_elements.end(); ++it)
	{
		if ((*it)->GetVisible())
			(*it)->DrawEditor();
	}
}

void ModuleEditor::AddEditorWindow(const char * name, EditorWindow * window)
{
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
			editor_windows.push_back(window);
		}
	}
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
	for (std::vector<EditorWindow*>::iterator it = editor_windows.begin(); it != editor_windows.end(); ++it)
	{
		if (ImGui::Begin((*it)->name.c_str(), &(*it)->opened))
		{
			(*it)->DrawEditor();

			ImGui::End();
		}
	}
}

void ModuleEditor::ImGuiInit()
{
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui_ImplSDL2_InitForOpenGL(App->window->GetWindow(), App->renderer3D->GetSDLGLContext());

	const char* glsl_version = "#version 130";
	ImGui_ImplOpenGL3_Init(glsl_version);

	LoadCustomStyle();

	font = io.Fonts->AddFontFromFileTTF("C:\\Users\\Guillem\\Documents\\GitHub\\BeEngine-2D\\source\\Resources\\fonts\\framd.ttf", 16);


	//ImGui::StyleColorsDark();
}

void ModuleEditor::ImGuiStartFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->GetWindow());
	ImGui::NewFrame();

	if (font != nullptr)
		ImGui::PushFont(font);
}

void ModuleEditor::ImGuiEndFrame()
{
	if (font != nullptr)
		ImGui::PopFont();
}

void ModuleEditor::ImGuiQuit()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void ModuleEditor::ToolsBar(float2 margins_left_up)
{
	float2 window_size = App->window->GetWindowSize();

	float2 tools_bar_pos = float2(margins_left_up.x, margins_left_up.y);
	float2 tools_bar_size = float2(window_size.x, 30);

	bool opened = true;

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus
		| ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoScrollbar;

	ImGui::SetNextWindowPos(ImVec2(tools_bar_pos.x, tools_bar_pos.y));
	ImGui::SetNextWindowSize(ImVec2(tools_bar_size.x, tools_bar_size.y));

	ImGui::Begin("Tools Bar", &opened, flags);

	ImGui::Button("Press to die");

	ImGui::End();
}

void ModuleEditor::DockingSpace(float2 margins_left_up, float2 margins_right_down)
{
	bool opened = true;

	float2 window_size = App->window->GetWindowSize();

	float2 docking_pos = float2(margins_left_up.x - 5, margins_left_up.y - 5);
	float2 docking_size = float2(window_size.x - margins_right_down.x + 7 - margins_left_up.x, 
		window_size.y - margins_right_down.y + 7 - margins_left_up.y);

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove 
		| ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus;

	ImGui::SetNextWindowPos(ImVec2(docking_pos.x, docking_pos.y));
	ImGui::SetNextWindowSize(ImVec2(docking_size.x, docking_size.y));

	ImGui::Begin("DockingSpace", &opened, flags);
	ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
	ImGui::DockSpace(dockspace_id, ImVec2(docking_size.x - 15, docking_size.y - 15));
	ImGui::End();
}

void ModuleEditor::LoadCustomStyle()
{
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 0.94f);
	colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.51f, 0.87f, 0.65f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.49f, 0.78f, 0.98f, 0.65f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.49f, 0.78f, 0.98f, 0.65f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.15f, 0.15f, 0.15f, 0.65f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.51f, 0.87f, 0.65f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.49f, 0.78f, 0.98f, 0.65f);
	colors[ImGuiCol_Button] = ImVec4(0.00f, 0.51f, 0.87f, 0.65f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.49f, 0.78f, 0.98f, 0.65f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 0.51f, 0.87f, 0.65f);
	colors[ImGuiCol_Header] = ImVec4(0.00f, 0.51f, 0.87f, 0.65f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.50f, 0.78f, 0.98f, 0.65f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 0.51f, 0.87f, 0.65f);
	colors[ImGuiCol_Separator] = ImVec4(0.00f, 0.51f, 0.87f, 0.65f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.49f, 0.78f, 0.98f, 0.65f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.00f, 0.51f, 0.87f, 0.65f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.51f, 0.87f, 0.65f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.49f, 0.78f, 0.98f, 0.65f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 0.51f, 0.87f, 0.65f);
	colors[ImGuiCol_Tab] = ImVec4(0.56f, 0.81f, 0.98f, 0.65f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.28f, 0.28f, 0.28f, 0.98f);
	colors[ImGuiCol_TabActive] = ImVec4(0.15f, 0.15f, 0.15f, 0.94f);
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

EditorWindow::EditorWindow()
{
}

std::string EditorWindow::GetName() const
{
	return name;
}

void EditorWindow::SetOpened(bool set)
{
	opened = set;
}

bool EditorWindow::GetOpened() const
{
	return opened;
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
