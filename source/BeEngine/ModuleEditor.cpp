#include "ModuleEditor.h"
#include "App.h"
#include "GeometryMath.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "MenuBar.h"
#include "ToolsBar.h"
#include "DockingSpace.h"
#include "SceneWindow.h"

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

	docking_space = (DockingSpace*)AddEditorElement(new DockingSpace(float2(0, 58), float2(0, 0)));
	menu_bar = (MenuBar*)AddEditorElement(new MenuBar(), true);
	tools_bar = (ToolsBar*)AddEditorElement(new ToolsBar(float2(0, 19)), true);

	AddEditorWindow("Scene", new SceneWindow());

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

	DrawEditorElements();
	DrawEditorWindows();

	ImGuiEndFrame();

	return ret;
}

bool ModuleEditor::Update()
{
	bool ret = true;

	ImGui::ShowDemoWindow(&demo_window_open);
	
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

	editor_fonts.clear();

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

			window->Start();
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
		ImGuiWindowFlags flags = 0;
		flags |= (*it)->GetWindowFlags();

		if (ImGui::Begin((*it)->name.c_str(), &(*it)->opened, flags))
		{
			ImVec2 win_pos = ImGui::GetWindowPos();
			ImVec2 win_size = ImGui::GetWindowSize();

			(*it)->window_size = float2(win_pos.x, win_pos.y);
			(*it)->window_size = float2(win_size.x, win_size.y);

			(*it)->DrawEditor();
		}
		ImGui::End();
	}
}

ImFont* ModuleEditor::GetLoadedFont(const char * name)
{
	ImFont* ret = nullptr;

	ret = editor_fonts[name];

	return ret;
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

	LoadImGuiFont("Roboto-Medium.ttf", 16, "RobotoMedium_16");
	LoadImGuiFont("Roboto-Medium.ttf", 18, "RobotoMedium_18");
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
	
	std::string font_path = App->GetBasePath() + std::string("fonts\\") + filename;

	ret = io.Fonts->AddFontFromFileTTF(font_path.c_str(), size);

	editor_fonts[load_name] = ret;

	return ret;
}

void ModuleEditor::LoadCustomStyle()
{
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.12f, 0.12f, 0.12f, 0.94f);
	colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.50f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.00f, 0.51f, 0.87f, 0.65f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.49f, 0.78f, 0.98f, 0.65f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.49f, 0.78f, 0.98f, 0.65f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.15f, 0.15f, 0.15f, 0.90f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.45f, 0.76f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.65f);
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
	colors[ImGuiCol_Separator] = ImVec4(0.29f, 0.29f, 0.29f, 0.65f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.00f, 0.51f, 0.87f, 0.65f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.00f, 0.51f, 0.87f, 0.65f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.51f, 0.87f, 0.65f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.49f, 0.78f, 0.98f, 0.65f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 0.51f, 0.87f, 0.65f);
	colors[ImGuiCol_Tab] = ImVec4(0.12f, 0.12f, 0.12f, 0.94f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.28f, 0.28f, 0.28f, 0.98f);
	colors[ImGuiCol_TabActive] = ImVec4(0.12f, 0.12f, 0.12f, 0.94f);
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

float2 EditorWindow::GetWindowSize() const
{
	return window_size;
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
