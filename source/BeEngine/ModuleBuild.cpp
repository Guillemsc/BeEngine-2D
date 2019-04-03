#include "ModuleBuild.h"
#include "ModuleFileSystem.h"
#include "App.h"
#include "ModuleProject.h"
#include "ModuleScripting.h"
#include "ModuleAssets.h"
#include "ModuleJson.h"
#include "ModuleEvent.h"
#include "ModuleResource.h"
#include "ResourceScene.h"

ModuleBuild::ModuleBuild()
{
}

ModuleBuild::~ModuleBuild()
{
}

bool ModuleBuild::Awake()
{
	bool ret = true;

	return ret;
}

bool ModuleBuild::Start()
{
	bool ret = true;

	App->event->Suscribe(std::bind(&ModuleBuild::OnEvent, this, std::placeholders::_1), EventType::RESOURCES_LOADED);

	return ret;
}

bool ModuleBuild::CleanUp()
{
	bool ret = true;

	App->event->UnSuscribe(std::bind(&ModuleBuild::OnEvent, this, std::placeholders::_1), EventType::RESOURCES_LOADED);

	return ret;
}

void ModuleBuild::OnLoadProject(JSON_Doc * config)
{
	scene_to_load = config->GetString("build.scene");
}

void ModuleBuild::OnLoadBuild(JSON_Doc * config)
{
	scene_to_load = config->GetString("scene");
}

bool ModuleBuild::GenerateBuild(const std::string & folder, std::vector<std::string>& errors)
{
	bool ret = false;

	bool initial_checks_correct = true;

	Project* proj = App->project->GetCurrProject();

	if (proj == nullptr)
	{
		std::string error = "There is not a selected project";
		errors.push_back(error);

		initial_checks_correct = false;
	}

	if (!App->file_system->FolderExists(folder))
	{
		std::string error = "Specified folder does not exist [" + folder + "]";
		errors.push_back(error);

		initial_checks_correct = false;
	}

	if (App->scripting->GetNeedsToCompileScripts())
	{
		std::string error = "Scripts need to be compiled";
		errors.push_back(error);

		initial_checks_correct = false;
	}

	if (!App->scripting->GetScriptsCompile())
	{
		std::string error = "Scripts do not compile";
		errors.push_back(error);

		initial_checks_correct = false;
	}

	if(initial_checks_correct)
	{
		if (proj != nullptr)
		{
			std::string project_name = proj->GetName();

			std::string new_folder;
			if(App->file_system->FolderCreate(folder, project_name, true, new_folder))
			{
				std::string root_exe_path = App->file_system->GetExecutableFilePath();

				DecomposedFilePath dfo_root_exe = App->file_system->DecomposeFilePath(root_exe_path);

				std::string new_exe_path_check = new_folder + dfo_root_exe.file_name + "." + dfo_root_exe.file_extension;

				bool exe_clear = true;

				if (App->file_system->FileExists(new_exe_path_check))
					exe_clear = App->file_system->FileDelete(new_exe_path_check);
								
				if (exe_clear)
				{
					bool exe_copied = false;

					exe_copied = App->file_system->FileCopyPaste(root_exe_path, new_folder, false);

					if (exe_copied)
					{
						std::string resources_folder = App->file_system->GetWorkingDirectory();

						bool engine_files_copied = App->file_system->FolderCopyPaste(resources_folder, new_folder, false);

						if (engine_files_copied)
						{
							std::string new_data_path;
							App->file_system->FolderCreate(new_folder, "data", false, new_data_path);

							std::string library_folder = App->assets->GetLibraryPath();

							bool library_files_copied = App->file_system->FolderCopyPaste(library_folder, new_data_path, false);

							if (library_files_copied)
							{
								bool build_file_created = CreateBuildFile(new_folder);
							}
						}
						else
						{
							std::string error = "There was a problem creating engine files";
							errors.push_back(error);
						}
					}
					else
					{
						std::string error = "Executable could not be created";
						errors.push_back(error);
					}
				}
				else
				{
					std::string error = "Another file has the same name as the executable and it could not be created";
					errors.push_back(error);
				}
			}
		}
	}

	return ret;
}

ResourceScene * ModuleBuild::GetResourceSceneToLoad() const
{
	return resource_scene_to_load;
}

void ModuleBuild::OnEvent(Event * ev)
{
	if (ev->GetType() == EventType::RESOURCES_LOADED)
	{
		if (scene_to_load.compare("") != 0)
		{
			resource_scene_to_load = (ResourceScene*)App->resource->GetResourceFromUid(scene_to_load, ResourceType::RESOURCE_TYPE_SCENE);
		}
	}
}

bool ModuleBuild::CreateBuildFile(const std::string & folder)
{
	bool ret = false;

	JSON_Doc* doc = App->json->CreateJSON(folder.c_str(), "build", "bebuild");

	if (doc != nullptr)
	{
		doc->SetString("scene", scene_to_load.c_str());

		doc->Save();
	}

	return ret;
}
