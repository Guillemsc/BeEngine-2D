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
#include "ModuleState.h"
#include "ModuleGameObject.h"
#include "ModuleEditor.h"
#include "GameWindow.h"
#include "ModuleWindow.h"

ModuleBuild::ModuleBuild()
{
}

ModuleBuild::~ModuleBuild()
{
}

bool ModuleBuild::Awake()
{
	bool ret = true;

	TryLoadBuildProject();

	return ret;
}

bool ModuleBuild::Start()
{
	bool ret = true;

	App->event->Suscribe(std::bind(&ModuleBuild::OnEvent, this, std::placeholders::_1), EventType::RESOURCE_SCRIPTS_FIELDS_CHANGED);
	App->event->Suscribe(std::bind(&ModuleBuild::OnEvent, this, std::placeholders::_1), EventType::RESOURCE_DESTROYED);

	return ret;
}

bool ModuleBuild::CleanUp()
{
	bool ret = true;

	App->event->UnSuscribe(std::bind(&ModuleBuild::OnEvent, this, std::placeholders::_1), EventType::RESOURCE_SCRIPTS_FIELDS_CHANGED);
	App->event->UnSuscribe(std::bind(&ModuleBuild::OnEvent, this, std::placeholders::_1), EventType::RESOURCE_DESTROYED);

	return ret;
}

void ModuleBuild::OnLoadProject(JSON_Doc * config)
{
	scene_to_load = config->GetString("build.scene");
	build_name = config->GetString("build.name");
}

void ModuleBuild::OnLoadBuild(JSON_Doc * config)
{

}

void ModuleBuild::SetResourceSceneToLoad(ResourceScene* scene)
{
	resource_scene_to_load = scene;
	
	std::string project_file = App->project->GetCurrProjectFilePath();

	scene_to_load = "";

	if (scene != nullptr)
		scene_to_load = scene->GetUID();
	
	JSON_Doc* doc = App->json->LoadJSON(project_file.c_str());

	if (doc != nullptr)
	{
		doc->SetString("build.scene", scene_to_load.c_str());

		doc->Save();

		App->json->UnloadJSON(doc);
	}
}

ResourceScene * ModuleBuild::GetResourceSceneToLoad() const
{
	return resource_scene_to_load;
}

void ModuleBuild::SetBuildName(const std::string & name)
{
	build_name = name;

	std::string project_file = App->project->GetCurrProjectFilePath();

	JSON_Doc* doc = App->json->LoadJSON(project_file.c_str());

	if (doc != nullptr)
	{
		doc->SetString("build.name", build_name.c_str());

		doc->Save();

		App->json->UnloadJSON(doc);
	}
}

std::string ModuleBuild::GetBuildName() const
{
	return build_name;
}

bool ModuleBuild::GenerateBuild(const std::string & folder, std::vector<std::string>& errors)
{
	bool ret = false;

	bool initial_checks_correct = true;

	Project* proj = App->project->GetCurrProject();

	if (build_name.compare("") == 0)
	{
		std::string error = "The build has no name";
		errors.push_back(error);

		initial_checks_correct = false;
	}

	if (resource_scene_to_load == nullptr)
	{
		std::string error = "The build has no scene to load";
		errors.push_back(error);

		initial_checks_correct = false;
	}

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
			if(App->file_system->FolderCreate(folder, build_name, true, new_folder))
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

					std::string new_exe_path;
					exe_copied = App->file_system->FileCopyPaste(root_exe_path, new_folder, false, new_exe_path);

					if (exe_copied)
					{
						bool exe_renamed = false;
						exe_renamed = App->file_system->FileRename(new_exe_path, build_name);

						if (exe_renamed)
						{
							std::string resources_folder = App->file_system->GetWorkingDirectory();

							bool engine_files_copied = App->file_system->FolderCopyPaste(resources_folder, new_folder, false);

							if (engine_files_copied)
							{
								std::string new_data_path;
								App->file_system->FolderCreate(new_folder, "data", false, new_data_path);

								std::string library_folder = App->assets->GetLibraryPath();

								App->file_system->FolderCopyPaste(library_folder, new_data_path, false);
								
								bool build_file_created = CreateBuildFile(new_folder);
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

void ModuleBuild::OnEvent(Event * ev)
{
	if (ev->GetType() == EventType::RESOURCE_SCRIPTS_FIELDS_CHANGED)
	{
		if (scene_to_load.compare("") != 0)
		{
			resource_scene_to_load = (ResourceScene*)App->resource->GetResourceFromUid(scene_to_load, ResourceType::RESOURCE_TYPE_SCENE);

			if (resource_scene_to_load != nullptr)
			{
				if (App->state->GetEngineState() == EngineState::ENGINE_STATE_BUILD)
				{
					App->gameobject->DestroyScene(App->gameobject->GetRootScene());
					resource_scene_to_load->LoadToScene(App->gameobject->GetRootScene());

					App->state->SetEditorUpdateState(EditorUpdateState::EDITOR_UPDATE_STATE_PLAY);
				}
			}
		}
	}
	else if (ev->GetType() == EventType::RESOURCE_DESTROYED)
	{
		EventResourceDestroyed* erd = (EventResourceDestroyed*)ev;

		if (resource_scene_to_load == erd->GetResource())
			SetResourceSceneToLoad(nullptr);
	}
}

bool ModuleBuild::CreateBuildFile(const std::string & folder)
{
	bool ret = false;

	JSON_Doc* doc = App->json->CreateJSON(folder.c_str(), "build", "bebuild");

	if (doc != nullptr)
	{
		doc->SetString("name", build_name.c_str());
		doc->SetString("scene", scene_to_load.c_str());

		doc->Save();

		App->json->UnloadJSON(doc);
	}

	return ret;
}

void ModuleBuild::TryLoadBuildProject()
{
	bool is_build = false;

	std::string build_project_path = App->file_system->GetWorkingDirectory() + "build.bebuild";

	if (App->file_system->FileExists(build_project_path))
	{
		JSON_Doc* doc = App->json->LoadJSON(build_project_path.c_str());

		if(doc != nullptr)
		{
			scene_to_load = doc->GetString("scene");
			build_name = doc->GetString("name");

			if (scene_to_load.compare("") != 0 && build_name.compare("") != 0)
			{
				is_build = true;
			}
		}
	}

	if (is_build)
	{
		App->state->SetEngineState(EngineState::ENGINE_STATE_BUILD);

		App->window->GetWindowNamer()->RemoveNamePart("app_version");

		App->SetAppName(build_name.c_str());

		std::string data_path = App->file_system->GetWorkingDirectory() + "data\\";

		EventEngineIsBuild* ev = new EventEngineIsBuild(data_path);
		App->event->SendEvent(ev);
	}
	else
	{
		App->state->SetEngineState(EngineState::ENGINE_STATE_EDITOR);

		EventEngineIsEditor* ev = new EventEngineIsEditor();
		App->event->SendEvent(ev);
	}
}
