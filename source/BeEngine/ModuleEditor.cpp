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

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

void ModuleEditor::ImGuiInit()
{
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

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
