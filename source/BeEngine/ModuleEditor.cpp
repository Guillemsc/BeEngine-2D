#include "ModuleEditor.h"
#include "App.h"
#include "ModuleWindow.h"
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

	ImGuiNewFrame();

	MenuBar();

	ToolsBar(float2(0, 19));

	DockingSpace(float2(0, 50), float2(0, 0));

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

	ImGui::StyleColorsDark();
}

void ModuleEditor::ImGuiNewFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->GetWindow());
	ImGui::NewFrame();
}

void ModuleEditor::ImGuiQuit()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void ModuleEditor::MenuBar()
{
	ImGui::BeginMainMenuBar();

	bool selected = true;
	bool enabled = true;
	ImGui::MenuItem("asd", "asd", selected, enabled);

	ImGui::EndMainMenuBar();
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
