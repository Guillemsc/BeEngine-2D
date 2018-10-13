#include "ModuleProject.h"
#include "App.h"
#include "ModuleFileSystem.h"
#include "ModuleJson.h"
#include "ModuleWindow.h"

ModuleProject::ModuleProject()
{
}

ModuleProject::~ModuleProject()
{
}

bool ModuleProject::Awake()
{
	bool ret = true;

	App->window->GetWindowNamer()->AddNamePart("project_name", "");

	LoadProjects();

	return ret;
}

bool ModuleProject::Start()
{
	bool ret = true;

	return ret;
}

bool ModuleProject::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool ModuleProject::Update()
{
	bool ret = true;

	return ret;
}

bool ModuleProject::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool ModuleProject::CleanUp()
{
	bool ret = true;

	return ret;
}

bool ModuleProject::CreateNewProject(const char* path, const char * name)
{
	bool ret = false;

	if (App->file_system->FolderExists(path))
	{
		Project* proj = new Project();

		proj->SetPath(path);
		proj->SetName(name);

		projects.push_back(proj);

		JSON_Doc* doc = App->json->CreateJSON(path, "project", "beproject");

		if (doc != nullptr)
		{
			doc->SetString("name", name);

			ret = true;
		}

		SerializeProjects();
	}

	return ret;
}

bool ModuleProject::LoadProject(const char * path)
{
	bool ret = false;

	std::string filepath = path + std::string("project.beproject");

	JSON_Doc* doc = App->json->LoadJSON(filepath.c_str());

	if (doc != nullptr)
	{
		std::string name = doc->GetString("name");
		
		Project* proj = new Project();
		proj->SetPath(path);
		proj->SetName(name.c_str());

		projects.push_back(proj);

		ret = true;
	}

	SerializeProjects();

	return ret;
}

void ModuleProject::SetCurrProject(Project * set)
{
	curr_project = set;

	if (curr_project != nullptr)
	{
		App->window->GetWindowNamer()->UpdateNamePart("project_name", curr_project->GetName());
	}
}

std::vector<Project*> ModuleProject::GetProjects() const
{
	return projects;
}

void ModuleProject::LoadProjects()
{
	projects_json_filepath = App->GetBasePath() + std::string("Projects.json");

	JSON_Doc* doc = App->json->LoadJSON(projects_json_filepath.c_str());

	if (doc == nullptr)
		doc = App->json->CreateJSON(projects_json_filepath.c_str());

	if (doc != nullptr)
	{
		int projects_count = doc->GetNumber("projects_count");

		for (int i = 0; i < projects_count; ++i)
		{
			JSON_Doc section_node = doc->GetNode();

			std::string proj_section = "project_" + std::to_string(i);

			if (section_node.MoveToSection(proj_section))
			{
				std::string name = section_node.GetString("name");
				std::string path = section_node.GetString("path");

				Project* proj = new Project();
				proj->SetName(name.c_str());
				proj->SetPath(path.c_str());

				projects.push_back(proj);
			}
		}

		App->json->UnloadJSON(doc);
	}
}

void ModuleProject::SerializeProjects()
{
	JSON_Doc* doc = App->json->LoadJSON(projects_json_filepath.c_str());

	if (doc == nullptr)
		doc = App->json->CreateJSON(projects_json_filepath.c_str());

	if (doc != nullptr)
	{
		doc->Clear();

		doc->SetNumber("projects_count", projects.size());

		int count = 0;
		for (std::vector<Project*>::iterator it = projects.begin(); it != projects.end(); ++it)
		{
			JSON_Doc section_node = doc->GetNode();

			std::string proj_section = "project_" + std::to_string(count);
			section_node.AddSection(proj_section);

			if (section_node.MoveToSection(proj_section))
			{
				section_node.SetString("name", (*it)->GetName().c_str());
				section_node.SetString("path", (*it)->GetPath().c_str());
			}

			++count;
		}

		doc->Save();

		App->json->UnloadJSON(doc);
	}
}

Project::Project()
{
}

Project::~Project()
{
}

void Project::SetPath(const char* _path)
{
	path = _path;
}

void Project::SetName(const char* _name)
{
	name = _name;
}

void Project::SetLastTimeOpened(const char * _date)
{
	last_time_opened = _date;
}

std::string Project::GetPath() const
{
	return path;
}

std::string Project::GetName() const
{
	return name;
}

std::string Project::GetLastTimeOpened() const
{
	return last_time_opened;
}
