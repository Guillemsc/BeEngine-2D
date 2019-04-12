#include "ProjectManager.h"
#include "App.h"
#include "ModuleWindow.h"
#include "ModuleProject.h"
#include "ModuleFileSystem.h"
#include "Event.h"
#include "ModuleEvent.h"
#include "ModuleTexture.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

ProjectManager::ProjectManager()
{
}

ProjectManager::~ProjectManager()
{
}

void ProjectManager::Start()
{
	big_font = App->editor->GetLoadedFont("RobotoMedium_60");
	medium_font = App->editor->GetLoadedFont("RobotoMedium_30");
	small_font = App->editor->GetLoadedFont("RobotoMedium_18");

	std::string logo_path = std::string("images\\BeEngineNoBack.png");
	logo_texture = App->texture->LoadTexture(logo_path);
}

void ProjectManager::CleanUp()
{

}

void ProjectManager::DrawEditor()
{
	margins = 60;

	window_size = App->window->GetWindowSize();

	float2 docking_pos = float2(0, 0);
	float2 docking_size = float2(window_size.x, window_size.y );

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove
		| ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus
		| ImGuiWindowFlags_NoScrollbar;

	bool opened = true;

	ImGui::PushFont(big_font);

	ImGui::SetNextWindowPos(ImVec2(docking_pos.x, docking_pos.y));
	ImGui::SetNextWindowSize(ImVec2(docking_size.x, docking_size.y));
	if (ImGui::Begin("ProjectManager", &opened, flags))
	{
		ImGui::SetCursorScreenPos(ImVec2(margins, 40));

		if (logo_texture != nullptr)
		{
			float2 size = logo_texture->GetSize() * 0.3f;
			ImGui::Image((ImTextureID)logo_texture->GetId(), ImVec2(size.x, size.y));
		}

		ImGui::PopFont();

		if (App->project->GetProjectsLoaded())
		{
			ImGui::SetCursorScreenPos(ImVec2(margins, 140));

			if (state == ProjectManagerState::SELECTING_PROJECT)
			{
				DrawProjectSelector();
			}

			if (state == ProjectManagerState::CREATING_PROJECT)
			{
				DrawProjectCreator();
			}
		}
	}

	ImGui::End();
}

void ProjectManager::DrawProjectSelector()
{
	ImGui::PushFont(medium_font);

	if (ImGui::Button("New Project"))
	{
		state = ProjectManagerState::CREATING_PROJECT;

		project_creation_location = App->GetPreferedPath();
		memset(project_creation_name, 0, 100);
	}

	ImGui::SameLine();

	if (ImGui::Button("Open Project"))
	{
		bool canceled = false;
		const char* filter[1] = { "*.beproject" };
		std::string open_path = App->file_system->SelectFileDilog(canceled, filter);

		if (!canceled)
		{
			DecomposedFilePath dfp = App->file_system->DecomposeFilePath(open_path);

			App->project->LoadProject(dfp.path.c_str());
		}
	}

	ImGui::SetCursorScreenPos(ImVec2(margins, 195));
	ImGui::Separator();

	ImGui::BeginGroup();

	ImGui::SetCursorPosX(margins + (20));

	ImGui::BeginChild("Child", ImVec2(window_size.x - (margins * 2) - (20 * 2), window_size.y - 190), false);

	ImGui::PopFont();

	std::vector<Project*> projects = App->project->GetProjects();

	if (projects.size() > 0)
	{
		int count = 0;
		for (std::vector<Project*>::reverse_iterator it = projects.rbegin(); it != projects.rend(); ++it)
		{
			ImGui::PushFont(medium_font);

			ImGui::PushID(count);

			ImGui::SetCursorPosY(40 + (count * 100));

			if (ImGui::Button((*it)->GetName().c_str()))
			{
				App->project->SetCurrProject((*it));

				App->editor->SetEditorState(EditorState::MAIN_ENGINE);

				App->event->SendEvent(new EventProjectSelected());
			}

			ImGui::PopFont();

			ImGui::PushFont(small_font);

			std::string path = "Path: " + (*it)->GetPath();

			ImGui::Text(path.c_str());

			ImGui::SameLine();

			if (ImGui::Button("Open Folder"))
			{
				App->OpenFolder((*it)->GetPath().c_str());
			}

			ImGui::SameLine();

			if (ImGui::Button("X"))
			{
				App->project->RemoveProject((*it)->GetPath().c_str());
			}

			ImGui::PopID();

			ImGui::SetCursorPosY(110 + (count * 100));

			ImGui::Separator();

			ImGui::PopFont();

			++count;
		}
	}
	else
	{
		ImGui::PushFont(medium_font);

		ImGui::Text("There are no projects");

		ImGui::PopFont();
	}

	ImGui::EndChild();

	ImGui::EndGroup();
}

void ProjectManager::DrawProjectCreator()
{
	ImGui::PushFont(medium_font);

	ImGui::Text("Create new project");

	ImGui::SetCursorScreenPos(ImVec2(margins, 195));
	ImGui::Separator();

	ImGui::SetCursorScreenPos(ImVec2(margins, 220));

	ImGui::SetCursorPosX(margins);

	ImGui::Text("Project Name:");

	ImGui::SetCursorPosX(margins);

	ImGui::InputText("", &project_creation_name[0], 100);

	ImGui::SetCursorPosX(margins);

	ImGui::SetCursorScreenPos(ImVec2(margins, 315));

	ImGui::Text("Location:");

	ImGui::SetCursorPosX(margins);

	ImGui::Text(project_creation_location.c_str());

	ImGui::SameLine();

	if (ImGui::Button("..."))
	{
		bool canceled = false;
		std::string folder = App->file_system->SelectFolderDialog(canceled);

		if (!canceled)
		{
			project_creation_location = folder;
		}
	}

	ImGui::SetCursorScreenPos(ImVec2(margins, 415));

	if(ImGui::Button("Create project"))
	{
		std::string name = project_creation_name;
		
		if (App->project->CreateNewProject(project_creation_location.c_str(), name.c_str()))
		{
			state = ProjectManagerState::SELECTING_PROJECT;
		}
	}

	ImGui::SameLine();

	if (ImGui::Button("Cancel"))
	{
		state = ProjectManagerState::SELECTING_PROJECT;
	}

	ImGui::PopFont();
}
