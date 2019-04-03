#include "BuildWindow.h"
#include "ModuleResource.h"
#include "App.h"
#include "ModuleBuild.h"
#include "ResourceScene.h"

BuildWindow::BuildWindow()
{
}

BuildWindow::~BuildWindow()
{
}

void BuildWindow::Start()
{
}

void BuildWindow::CleanUp()
{
}

void BuildWindow::DrawEditor()
{
	ImGui::Text("Build name");

	ImGui::SameLine();

	std::string build_name = App->build->GetBuildName();

	char name_arr[51];
	strcpy_s(name_arr, 51, build_name.c_str());
	if (ImGui::InputText("", name_arr, 50, ImGuiInputTextFlags_AutoSelectAll))
		App->build->SetBuildName(name_arr);

	Resource* starting_scene = App->build->GetResourceSceneToLoad();

	if (resource_selector.Draw("Starting scene", ResourceType::RESOURCE_TYPE_SCENE, starting_scene))
	{
		ResourceScene* starting_scene_changed = (ResourceScene*)starting_scene;

		App->build->SetResourceSceneToLoad(starting_scene_changed);
	}

	ImGui::Separator();

	if (ImGui::Button("Generate build"))
	{
		bool cancelled = false;
		std::string folder = App->file_system->SelectFolderDialog(cancelled);

		if (!cancelled)
		{
			std::vector<std::string> errors;
			App->build->GenerateBuild(folder, errors);
		}
	}
}

ImGuiWindowFlags BuildWindow::GetWindowFlags()
{
	return ImGuiWindowFlags();
}

void BuildWindow::Save()
{
}
