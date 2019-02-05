#include "ModuleAssets.h"
#include "ModuleResource.h"
#include "Functions.h"
#include "App.h"
#include "ModuleFileSystem.h"
#include "ModuleEvent.h"
#include "ModuleProject.h"
#include "ModuleEditor.h"
#include "ModuleScripting.h" 
#include "ScriptingObjectFileWatcher.h"
#include "ModuleJson.h"
#include "ModuleInput.h"
#include "ResourceTexture.h"
#include "ResourceScript.h"
#include "ScriptingObjectCompiler.h"
#include "ModuleInput.h"
#include "ModuleScripting.h"
#include "ScriptingObjectSolutionManager.h"
#include "Event.h"

ModuleAssets::ModuleAssets()
{
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
}

ModuleAssets::~ModuleAssets()
{
	ilShutDown();
}

bool ModuleAssets::Awake()
{
	bool ret = true;

	App->event->Suscribe(std::bind(&ModuleAssets::OnEvent, this, std::placeholders::_1), EventType::PROJECT_SELECTED);
	App->event->Suscribe(std::bind(&ModuleAssets::OnEvent, this, std::placeholders::_1), EventType::TIME_SLICED_TASK_FINISHED);
	App->event->Suscribe(std::bind(&ModuleAssets::OnEvent, this, std::placeholders::_1), EventType::WATCH_FILE_FOLDER);
	App->event->Suscribe(std::bind(&ModuleAssets::OnEvent, this, std::placeholders::_1), EventType::SCRIPTS_COMPILED);

	return ret;
}

bool ModuleAssets::Start()
{
	bool ret = true;

	return ret;
}

bool ModuleAssets::PreUpdate()
{
	bool ret = true;

	ManageFoldersToCheck();

	return ret;
}

bool ModuleAssets::Update()
{
	bool ret = true;

	return ret;
}

bool ModuleAssets::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool ModuleAssets::CleanUp()
{
	bool ret = true;

	App->event->UnSuscribe(std::bind(&ModuleAssets::OnEvent, this, std::placeholders::_1), EventType::PROJECT_SELECTED);
	App->event->UnSuscribe(std::bind(&ModuleAssets::OnEvent, this, std::placeholders::_1), EventType::TIME_SLICED_TASK_FINISHED);
	App->event->UnSuscribe(std::bind(&ModuleAssets::OnEvent, this, std::placeholders::_1), EventType::WATCH_FILE_FOLDER);
	App->event->UnSuscribe(std::bind(&ModuleAssets::OnEvent, this, std::placeholders::_1), EventType::SCRIPTS_COMPILED);

	return ret;
}

void ModuleAssets::OnEvent(Event* ev)
{
	switch (ev->GetType())
	{
	case EventType::PROJECT_SELECTED:
	{
		App->file_system->FolderCreate(App->project->GetCurrProjectBasePath().c_str(), "assets", false, assets_folder);
		App->file_system->FolderCreate(App->project->GetCurrProjectBasePath().c_str(), "library", false, library_folder);

		current_assets_folder = assets_folder;

		CreateLibraryFolders();

		StartLoadResourcesTimeSlicedTask();

		StartWatchingFolders();

		break;
	}
	case EventType::TIME_SLICED_TASK_FINISHED:
	{
		EventTimeSlicedTaskFinished* ett = (EventTimeSlicedTaskFinished*)ev;

		if (ett->GetTask() == time_sliced_task_loading_resources)
		{
			App->scripting->ForceCompileScripts();
		}

		break;
	}
	case EventType::WATCH_FILE_FOLDER:
	{
		EventWatchFileFolderChanged* efc = (EventWatchFileFolderChanged*)ev;

		DecomposedFilePath df = efc->GetPath();

		AddToFoldersToCheck(df.path);

		break;
	}
	case EventType::SCRIPTS_COMPILED:
	{
		EventScriptsCompiled* esc = (EventScriptsCompiled*)ev;

		if (esc->GetCompiles())
			LoadUserScriptsInfo();

		break;
	}
	default:
		break;
	}
}

std::string ModuleAssets::GetNewUID()
{
	return GetUIDRandomHexadecimal();
}

void ModuleAssets::SetCurrentAssetsPath(const char * path)
{
	current_assets_folder = path;
}

std::string ModuleAssets::GetCurrentAssetsPath()
{
	return current_assets_folder;
}

std::string ModuleAssets::GetLibraryPath()
{
	return library_folder;
}

std::string ModuleAssets::GetAssetsPath()
{
	return assets_folder;
}

bool ModuleAssets::LoadFileToEngine(const char * filepath)
{
	bool ret = false;

	if (App->file_system->FileExists(filepath))
	{
		bool can_load = CanLoadFile(filepath);

		if (can_load)
		{
			std::string new_path = filepath;

			App->file_system->FileCopyPaste(filepath, current_assets_folder.c_str(), true, new_path);

			ForceUpdateFolders();
		}
	}

	return ret;
}

bool ModuleAssets::ManageModifiedAsset(const char * filepath)
{
	bool ret = false;

	DecomposedFilePath dfp = App->file_system->DecomposeFilePath(filepath);

	bool can_load = App->assets->CanLoadFile(filepath);
	bool is_meta = App->assets->IsMeta(filepath);
	bool exists = App->file_system->FileExists(filepath);

	bool to_delete = false;

	if (!is_meta)
	{
		if (can_load)
		{
			if (exists)
			{
				Resource* loaded_res = nullptr;
				ExportAssetToLibrary(filepath);
			}
			else
			{
				UnloadAssetFromEngine(filepath);
			}

			if (dfp.file_extension_lower_case == "cs")
				App->scripting->CompileScripts();
		}
		else
		{
			if (exists)
				to_delete = true;
		}
	}
	else
	{
		if (exists)
		{
			std::string asset_filepath = GetAssetFileFromMeta(filepath);

			if (App->file_system->FileExists(asset_filepath.c_str()))
			{
				App->assets->ExportAssetToLibrary(filepath);
			}
			else
				to_delete = true;
		}
		else
		{
			std::string asset_filepath = GetAssetFileFromMeta(filepath);

			App->assets->ExportAssetToLibrary(asset_filepath.c_str());
		}
	}

	if (to_delete)
	{
			App->assets->StopRisingWatchingEvents();

			App->file_system->FileDelete(filepath);

			App->assets->StartRisingWatchingEvents();
	}

	return ret;
}

void ModuleAssets::UnloadAssetFromEngine(const char * filepath)
{
	Resource* res = App->resource->GetResourceFromAssetFile(filepath);

	if (res != nullptr)
	{
		res->EM_RemoveAsset();

		App->resource->DestroyResource(res);
	}
}

bool ModuleAssets::ExportAssetToLibrary(const char * filepath)
{
	bool ret = false;

	Resource* res = App->resource->GetResourceFromAssetFile(filepath);

	if (res == nullptr)
	{
		DecomposedFilePath dfp = App->file_system->DecomposeFilePath(filepath);

		ResourceType type = App->resource->GetResourceTypeFromAssetExtension(dfp.file_extension_lower_case.c_str());

		if (type != ResourceType::RESOURCE_TYPE_UNKWNOWN)
		{
			res = App->resource->CreateResource(type);

			if (res != nullptr)
			{
				res->EM_InitResource(filepath);

				ret = true;
			}
		}
	}
	else
	{
		res->EM_ExportToLibrary();

		ret = true;
	}

	return ret;
}

bool ModuleAssets::ImportAsset(const char * filepath, Resource*& res)
{
	bool ret = false;

	res = App->resource->GetResourceFromAssetFile(filepath);

	if (res == nullptr)
	{
		DecomposedFilePath dfp = App->file_system->DecomposeFilePath(filepath);

		ResourceType type = App->resource->GetResourceTypeFromAssetExtension(dfp.file_extension_lower_case.c_str());

		if (type != ResourceType::RESOURCE_TYPE_UNKWNOWN)
		{
			res = App->resource->CreateResource(type);

			if (res != nullptr)
			{
				res->EM_InitResource(filepath);

				ret = true;
			}
		}
	}
	else
	{
		res->EM_ImportFromLibrary();
	}

	return ret;
}

bool ModuleAssets::RenameAsset(const char * filepath, const char * new_name)
{
	bool ret = false;

	Resource* res = App->resource->GetResourceFromAssetFile(filepath);

	if (res != nullptr)
	{
		res->EM_RenameAsset(new_name);
	}

	return ret;
}

bool ModuleAssets::MoveAsset(const char * filepath, const char * new_path)
{
	bool ret = false;

	if (App->file_system->FolderExists(new_path) && App->file_system->FileExists(filepath))
	{
		Resource* res = App->resource->GetResourceFromAssetFile(filepath);

		if (res != nullptr)
		{
			res->EM_MoveAsset(new_path);

			ret = true;
		}
	}

	return ret;
}

bool ModuleAssets::CreateScript(const char * filepath, const char * name)
{
	bool ret = false;

	std::string asset_path;
	if (App->scripting->compiler->CreateScriptFromTemplate(filepath, name, asset_path))
	{
		ExportAssetToLibrary(asset_path.c_str());
	}

	App->scripting->CompileScripts();

	return ret;
}

bool ModuleAssets::MoveAssetsFolder(const char * folder, const char * new_path)
{
	bool ret = false;

	if (App->file_system->FolderExists(new_path) && App->file_system->FolderExists(folder))
	{
		std::string parent_path = App->file_system->FolderParent(folder);

		if (parent_path.compare(new_path) != 0)
		{
			DecomposedFilePath curr_folder_dfp = App->file_system->DecomposeFilePath(folder);

			std::string new_path_created;
			App->file_system->FolderCreate(new_path, curr_folder_dfp.folder_name.c_str(), true, new_path_created);

			std::vector<std::string> files_folders_on_folder = App->file_system->GetFilesAndFoldersInPath(folder);

			for (std::vector<std::string>::iterator it = files_folders_on_folder.begin(); it != files_folders_on_folder.end(); ++it)
			{
				DecomposedFilePath dfp = App->file_system->DecomposeFilePath((*it));

				if (dfp.its_folder)
				{
					MoveAssetsFolder(dfp.path.c_str(), new_path_created.c_str());
				}
				else
				{
					bool moved = MoveAsset(dfp.file_path.c_str(), new_path_created.c_str());

					if (!moved)
					{
						if (App->file_system->FileExists(dfp.file_path.c_str()))
							App->file_system->FileDelete(dfp.file_path.c_str());
					}
				}
			}

			App->file_system->FolderDelete(folder);

			ret = true;
		}
	}

	return ret;
}

bool ModuleAssets::CreateAssetsFolder(const char * path, const char * name)
{
	bool ret = false;

	ret = App->file_system->FolderCreate(path, name, true);

	return ret;
}

bool ModuleAssets::RenameAssetsFolder(const char* folder_path, const char * new_name)
{
	bool ret = false;

	ret = App->file_system->FolderRename(folder_path, new_name, true);

	return ret;
}

bool ModuleAssets::DeleteAssetsFolder(const char * folder)
{
	bool ret = false;

	if (App->file_system->FolderExists(folder))
	{
		std::vector<std::string> files_folders = App->file_system->GetFilesAndFoldersInPath(folder);

		for (std::vector<std::string>::iterator it = files_folders.begin(); it != files_folders.end(); ++it)
		{
			DecomposedFilePath dfp = App->file_system->DecomposeFilePath((*it));

			if (!IsMeta(dfp.file_path.c_str()));
			{
				if (dfp.its_folder)
				{
					DeleteAssetsFolder(dfp.path.c_str());
				}
				else
				{
					UnloadAssetFromEngine(dfp.file_path.c_str());
				}
			}
		}

		App->file_system->FolderDelete(folder);

		ret = true;
	}

	return ret;
}

void ModuleAssets::ForceUpdateFolders()
{
	force_update_folders = true;
}

void ModuleAssets::LoadUserScriptsInfo()
{
	if (App->scripting->user_code_assembly != nullptr && App->scripting->user_code_assembly->GetAssemblyLoaded())
	{
		if (App->scripting->scripting_assembly != nullptr && App->scripting->scripting_assembly->GetAssemblyLoaded())
		{
			ScriptingClass be_engine_reference_class;
			if (App->scripting->scripting_assembly->GetClass("BeEngine", "BeEngineScript", be_engine_reference_class))
			{
				std::vector<Resource*> script_resources = App->resource->GetResourcesFromResourceType(ResourceType::RESOURCE_TYPE_SCRIPT);

				for (std::vector<Resource*>::iterator it = script_resources.begin(); it != script_resources.end(); ++it)
				{
					ResourceScript* curr_script = (ResourceScript*)(*it);

					curr_script->ClearScriptFields();

					std::string script_name = curr_script->GetDecomposedAssetFilepath().file_name;

					curr_script->inherits_from_beengine_reference = false;

					ScriptingClass sc;
					if (App->scripting->user_code_assembly->GetClass("", script_name.c_str(), sc))
					{
						curr_script->inherits_from_beengine_reference = sc.GetIsInheritedFrom(be_engine_reference_class);

						if (curr_script->inherits_from_beengine_reference)
						{
							curr_script->script_class = sc;

							std::map<std::string, MonoType*> fields = sc.GetFields();

							for (std::map<std::string, MonoType*>::iterator it = fields.begin(); it != fields.end(); ++it)
							{
								std::string type_name = mono_type_get_name((*it).second);

								if (type_name.compare("System.String") == 0)
								{
									curr_script->AddStringScriptField((*it).first);
								}
								else if (type_name.compare("System.Int32") == 0)
								{
									curr_script->AddIntScriptField((*it).first);
								}
								else if (type_name.compare("System.Single") == 0)
								{
									curr_script->AddFloatScriptField((*it).first);
								}
								else if (type_name.compare("System.Boolean") == 0)
								{
									curr_script->AddBoolScriptField((*it).first);
								}
							}
						}
					}

					EventResourceScriptsFieldsChanged* ersfc = new EventResourceScriptsFieldsChanged();
					App->event->SendEvent(ersfc);
				}
			}
		}
	}
}

bool ModuleAssets::CanLoadFile(const char * filepath)
{
	bool ret = false;

	DecomposedFilePath dfp = App->file_system->DecomposeFilePath(filepath);

	ResourceType type = App->resource->GetResourceTypeFromAssetExtension(dfp.file_extension_lower_case.c_str());

	if (type != ResourceType::RESOURCE_TYPE_UNKWNOWN)
		ret = true;

	return ret;
}

bool ModuleAssets::IsMeta(const char * filepath)
{
	bool ret = false;

	DecomposedFilePath df = App->file_system->DecomposeFilePath(filepath);

	if (df.file_extension_lower_case.compare("meta") == 0)
		ret = true;

	return ret;
}

std::string ModuleAssets::GetAssetFileFromMeta(const char * metapath)
{
	DecomposedFilePath df = App->file_system->DecomposeFilePath(metapath);

	std::string asset_path = df.path + df.file_name;

	return asset_path;
}

std::string ModuleAssets::GetMetaFileFromAsset(const char * filepath)
{
	std::string ret = filepath + std::string(".meta");
	return ret;
}

void ModuleAssets::StartWatchingFolders()
{
	App->scripting->file_watcher->Watch(GetAssetsPath().c_str());
}

void ModuleAssets::StopWatchingFolders()
{
	App->scripting->file_watcher->StopWatch(GetAssetsPath().c_str());
}

void ModuleAssets::StopRisingWatchingEvents()
{
	--rising_watching_events_index;

	if (rising_watching_events_index < 0)
		rising_watching_events_index = 0;

	if (rising_watching_events_index == 0)
	{
		App->scripting->file_watcher->SetRiseEvents(false);
	}
}

void ModuleAssets::StartRisingWatchingEvents()
{
	if (rising_watching_events_index == 0)
		App->scripting->file_watcher->SetRiseEvents(true);

	++rising_watching_events_index;
}

void ModuleAssets::CreateLibraryFolders()
{
	std::map<ResourceType, std::string> resource_names = App->resource->GetAllResourcesNames();

	if (App->file_system->FolderExists(library_folder.c_str()))
	{
		for (std::map<ResourceType, std::string>::iterator it = resource_names.begin(); it != resource_names.end(); ++it)
		{
			App->file_system->FolderCreate(library_folder.c_str(), (*it).second.c_str());
		}
	}
}

void ModuleAssets::AddToFoldersToCheck(std::string folder)
{
	bool exists = false;

	folders_to_update_timer.Start();

	for (std::vector<std::string>::iterator it = folders_to_update.begin(); it != folders_to_update.end(); ++it)
	{
		if (folder.compare((*it)) == 0)
		{
			exists = true;
			break;
		}
	}

	if (!exists)
		folders_to_update.push_back(folder);
}

void ModuleAssets::ManageFoldersToCheck()
{
	bool update = false;

	if (folders_to_update_timer.ReadSec() > 3)
		update = true;

	if (force_update_folders && folders_to_update.size() > 0)
	{
		update = true;

		force_update_folders = false;
	}

	if (update)
	{
		for (std::vector<std::string>::iterator it = folders_to_update.begin(); it != folders_to_update.end(); ++it)
		{
			std::string curr_folder = (*it);

			StartManageModifiedAssetsTimeSlicedTask(current_assets_folder);
		}

		folders_to_update.clear();
	}
}

void ModuleAssets::StartLoadResourcesTimeSlicedTask()
{
	time_sliced_task_loading_resources = new LoadResourcesTimeSlicedTask();
	App->time_sliced->StartTimeSlicedTask(time_sliced_task_loading_resources);
}

void ModuleAssets::StartManageModifiedAssetsTimeSlicedTask(const std::string & folder)
{
	time_sliced_task_manage_modified_assets = new ManageModifiedAssetsTimeSlicedTask(folder);
	App->time_sliced->StartTimeSlicedTask(time_sliced_task_manage_modified_assets);
}

LoadResourcesTimeSlicedTask::LoadResourcesTimeSlicedTask()
	: TimeSlicedTask(TimeSlicedTaskType::FOCUS, 4)
{

}

void LoadResourcesTimeSlicedTask::Start()
{
	state = LoadResourcesState::CHECK_ASSET_FILES_IMPORT;

	all_asset_files = App->file_system->GetFilesInPathAndChilds(App->assets->GetAssetsPath().c_str());
	all_library_files = App->file_system->GetFilesInPathAndChilds(App->assets->GetLibraryPath().c_str());

	asset_files_to_check = all_asset_files;
	all_asset_files_to_check_count = asset_files_to_check.size();
}

void LoadResourcesTimeSlicedTask::Update()
{
	switch (state)
	{
	case LoadResourcesState::CHECK_ASSET_FILES_IMPORT:
		CheckAssetFilesImport();
		break;
	case LoadResourcesState::CLEAN_ASSET_FOLDER:
		CleanAssetFolder();
		break;
	case LoadResourcesState::CLEAN_LIBRARY_FOLDER:
		CleanLibraryFolder();
		break;
	case LoadResourcesState::DELETE_UNNECESSARY_FILES:
		DeleteUnnecessaryFiles();
		break;
	default:
		break;
	}
}

void LoadResourcesTimeSlicedTask::Finish()
{
}

void LoadResourcesTimeSlicedTask::CheckAssetFilesImport()
{
	if (!asset_files_to_check.empty())
	{
		float progress = 100 - (100.0f / (float)all_asset_files_to_check_count) * (float)asset_files_to_check.size();
		progress *= 0.25f;
		progress += 0;
		SetPercentageProgress(progress);
		SetDescription("Loading resources");

		std::string curr_file = *asset_files_to_check.begin();

		bool is_meta = App->assets->IsMeta(curr_file.c_str());

		if (!is_meta)
		{
			bool can_load = App->assets->CanLoadFile(curr_file.c_str());

			if (can_load)
			{
				Resource* loaded_res = nullptr;
				App->assets->ImportAsset(curr_file.c_str(), loaded_res);

				if (loaded_res != nullptr)
				{
					asset_files_used.push_back(loaded_res->GetAssetFilepath());
					asset_files_used.push_back(loaded_res->GetMetaFilepath());
					library_files_used.push_back(loaded_res->GetLibraryFilepath());
				}
			}
		}

		asset_files_to_check.erase(asset_files_to_check.begin());
	}
	else
	{
		asset_files_to_check = all_asset_files;

		state = LoadResourcesState::CLEAN_ASSET_FOLDER;
	}
}

void LoadResourcesTimeSlicedTask::CleanAssetFolder()
{
	if (!asset_files_to_check.empty())
	{
		float progress = 100 - (100.0f / (float)all_asset_files_to_check_count) * (float)asset_files_to_check.size();
		progress *= 0.25f;
		progress += 25;
		SetPercentageProgress(progress);
		SetDescription("Cleaning asset folder");

		std::string curr_file = *asset_files_to_check.begin();

		bool used = false;

		for (std::vector<std::string>::iterator it = asset_files_used.begin(); it != asset_files_used.end(); ++it)
		{
			if ((*it).compare(curr_file) == 0)
			{
				asset_files_used.erase(it);
				used = true;
				break;
			}
		}

		if (!used)
		{
			files_to_delete.push_back(curr_file);
		}

		asset_files_to_check.erase(asset_files_to_check.begin());
	}
	else
	{
		library_files_to_check = all_library_files;
		all_library_files_to_check_count = library_files_to_check.size();

		state = LoadResourcesState::CLEAN_LIBRARY_FOLDER;
	}
}

void LoadResourcesTimeSlicedTask::CleanLibraryFolder()
{
	if (!library_files_to_check.empty())
	{
		float progress = 100 - (100.0f / (float)all_library_files_to_check_count) * (float)library_files_to_check.size();
		progress *= 0.25f;
		progress += 50;
		SetPercentageProgress(progress);
		SetDescription("Cleaning library folder");

		std::string curr_file = *library_files_to_check.begin();

		bool used = false;

		for (std::vector<std::string>::iterator it = library_files_used.begin(); it != library_files_used.end(); ++it)
		{
			if ((*it).compare(curr_file) == 0)
			{
				library_files_used.erase(it);
				used = true;
				break;
			}
		}

		if (!used)
		{
			files_to_delete.push_back(curr_file);
		}

		library_files_to_check.erase(library_files_to_check.begin());
	}
	else
	{
		all_files_to_delete_count = files_to_delete.size();
		state = LoadResourcesState::DELETE_UNNECESSARY_FILES;
	}
}

void LoadResourcesTimeSlicedTask::DeleteUnnecessaryFiles()
{
	if (!files_to_delete.empty())
	{
		float progress = 100 - (100.0f / (float)all_files_to_delete_count) * (float)files_to_delete.size();
		progress *= 0.25f;
		progress += 75;
		SetPercentageProgress(progress);
		SetDescription("Deleting unnecessary files");

		std::string curr_file = *files_to_delete.begin();

		App->assets->StopWatchingFolders();

		App->file_system->FileDelete(curr_file.c_str());

		App->assets->StartWatchingFolders();

		files_to_delete.erase(files_to_delete.begin());
	}
	else
	{
		FinishTask();
	}
}

ManageModifiedAssetsTimeSlicedTask::ManageModifiedAssetsTimeSlicedTask(std::string folder_to_check)
	: TimeSlicedTask(TimeSlicedTaskType::FOCUS, 4)
{
	this->folder_to_check = folder_to_check;
}

void ManageModifiedAssetsTimeSlicedTask::Start()
{	
	state = ManageModifiedAssetsState::CHECK_EXISTING_ASSETS;

	all_asset_files = App->file_system->GetFilesInPath(folder_to_check);
	all_asset_resources = App->resource->GetResourcesOnAssetsPath(folder_to_check);

	asset_files_to_check = all_asset_files;
	asset_resources_to_check = all_asset_resources;
}

void ManageModifiedAssetsTimeSlicedTask::Update()
{
	switch (state)
	{
	case ManageModifiedAssetsTimeSlicedTask::CHECK_EXISTING_ASSETS:
		CheckExisitingAssets();
		break;
	case ManageModifiedAssetsTimeSlicedTask::CHECK_FOR_DELETED_ASSETS:
		CheckForDeletedAssets();
		break;
	}
}

void ManageModifiedAssetsTimeSlicedTask::Finish()
{
}

void ManageModifiedAssetsTimeSlicedTask::CheckExisitingAssets()
{
	if (!asset_files_to_check.empty())
	{
		float progress = 100 - (100.0f / (float)all_asset_files.size()) * (float)asset_files_to_check.size();
		progress *= 0.5f;
		progress += 0;
		SetPercentageProgress(progress);
		SetDescription("Updating files");

		std::string curr_file = *asset_files_to_check.begin();

		App->assets->ManageModifiedAsset(curr_file.c_str());

		asset_files_to_check.erase(asset_files_to_check.begin());
	}
	else
	{
		state = ManageModifiedAssetsTimeSlicedTask::CHECK_FOR_DELETED_ASSETS;
	}
}

void ManageModifiedAssetsTimeSlicedTask::CheckForDeletedAssets()
{
	if (!asset_resources_to_check.empty())
	{
		float progress = 100 - (100.0f / (float)all_asset_resources.size()) * (float)asset_resources_to_check.size();
		progress *= 0.5f;
		progress += 50;
		SetPercentageProgress(progress);
		SetDescription("Checking deleted resources");

		Resource* curr_resource = *asset_resources_to_check.begin();

		std::string asset_path = curr_resource->GetAssetFilepath();

		if (!App->file_system->FileExists(asset_path))
		{
			curr_resource->EM_RemoveAsset();
			App->resource->DestroyResource(curr_resource);
		}

		asset_resources_to_check.erase(asset_resources_to_check.begin());
	}
	else
	{
		FinishTask();
	}
}

