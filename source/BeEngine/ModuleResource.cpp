#include "ModuleResource.h"
#include "Functions.h"
#include "App.h"
#include "ModuleFileSystem.h"
#include "ResourceTextureLoader.h"
#include "ResourceScriptLoader.h"
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

#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")

ModuleResource::ModuleResource()
{
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
}

ModuleResource::~ModuleResource()
{
	ilShutDown();
}

bool ModuleResource::Awake()
{
	bool ret = true;

	App->event->Suscribe(std::bind(&ModuleResource::OnEvent, this, std::placeholders::_1), EventType::PROJECT_SELECTED);
	App->event->Suscribe(std::bind(&ModuleResource::OnEvent, this, std::placeholders::_1), EventType::TIME_SLICED_TASK_FINISHED);
	App->event->Suscribe(std::bind(&ModuleResource::OnEvent, this, std::placeholders::_1), EventType::WATCH_FILE_FOLDER);

	AddResourceName(ResourceType::TEXTURE, "textures");
	AddResourceName(ResourceType::SCRIPT, "scripts");

	AddAssetExtension(ResourceType::TEXTURE, "png");
	AddAssetExtension(ResourceType::TEXTURE, "jpg");

	AddAssetExtension(ResourceType::SCRIPT, "cs");

	return ret;
}

bool ModuleResource::Start()
{
	bool ret = true;

	return ret;
}

bool ModuleResource::PreUpdate()
{
	bool ret = true;

	ManageFilesToCheck();

	return ret;
}

bool ModuleResource::Update()
{
	bool ret = true;

	return ret;
}

bool ModuleResource::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool ModuleResource::CleanUp()
{
	bool ret = true;

	DestroyAllResources();

	return ret;
}

std::string ModuleResource::GetNewUID()
{	
	return GetUIDRandomHexadecimal();
}

void ModuleResource::SetCurrentAssetsPath(const char * path)
{
	current_assets_folder = path;
}

std::string ModuleResource::GetCurrentAssetsPath()
{
	return current_assets_folder;
}

std::string ModuleResource::GetLibraryPath()
{
	return library_folder;
}

std::string ModuleResource::GetAssetsPath()
{
	return assets_folder;
}

std::string ModuleResource::GetResourceNameFromResourceType(const ResourceType & type)
{
	return resource_names[type];
}

std::string ModuleResource::GetLibraryPathFromResourceType(const ResourceType & type)
{
	std::string ret = "";

	ret += library_folder;

	ret += resource_names[type] + "\\";

	return ret;
}

Resource* ModuleResource::CreateResource(const ResourceType type)
{
	Resource* ret = nullptr;

	switch (type)
	{
	case ResourceType::TEXTURE:

		ret = new ResourceTexture();
		
		break;

	case ResourceType::SCRIPT:

		ret = new ResourceScript();

		break;
	default:
		break;
	}

	if (ret != nullptr)
	{
		std::map<ResourceType, std::vector<Resource*>>::iterator it = resources.find(type);

		if (it != resources.end())
		{
			it->second.push_back(ret);
		}
		else
		{
			std::vector<Resource*> new_vec;
			new_vec.push_back(ret);

			resources[type] = new_vec;
		}
	}

	return ret;
}

void ModuleResource::DestroyResource(Resource * res)
{
	for (std::map<ResourceType, std::vector<Resource*>>::iterator it = resources.begin(); it != resources.end(); ++it)
	{
		bool found = false;
		for (std::vector<Resource*>::iterator t = (*it).second.begin(); t != (*it).second.end(); ++t)
		{
			if ((*t) == res)
			{
				res->CleanUp();
				RELEASE(res);
				(*it).second.erase(t);
				found = true;
				break;
			}
		}

		if (found)
			break;
	}
}

Resource* ModuleResource::GetResourceFromAssetFile(const char* filepath)
{
	Resource* ret = nullptr;

	for (std::map<ResourceType, std::vector<Resource*>>::iterator it = resources.begin(); it != resources.end(); ++it)
	{
		bool found = false;
		for (std::vector<Resource*>::iterator t = (*it).second.begin(); t != (*it).second.end(); ++t)
		{
			if ((*t)->asset_filepath.compare(filepath) == 0)
			{
				ret = (*t);
				found = true;
				break;
			}
		}

		if (found)
			break;
	}

	return ret;
}

std::map<ResourceType, std::vector<Resource*>> ModuleResource::GetAllResources() const
{
	return resources;
}

bool ModuleResource::LoadFileToEngine(const char * filepath)
{
	bool ret = false;

	StopWatchingFolders();

	if (App->file_system->FileExists(filepath))
	{		
		bool can_load = CanLoadFile(filepath);

		if (can_load)
		{
			std::string new_path;
			bool can_load = App->file_system->FileCopyPaste(filepath, current_assets_folder.c_str(), false, new_path);

			if (can_load)
			{
				ret = ExportAssetToLibrary(new_path.c_str());
			}
		}
	}

	StartWatchingFolders();

	return ret;
}

void ModuleResource::UnloadAssetFromEngine(const char * filepath)
{
	StopWatchingFolders();

	Resource* res = GetResourceFromAssetFile(filepath);

	if (res != nullptr)
	{
		res->EM_RemoveAsset();

		DestroyResource(res);
	}

	StartWatchingFolders();
}

bool ModuleResource::ExportAssetToLibrary(const char * filepath)
{
	bool ret = false;

	StopWatchingFolders();

	Resource* res = GetResourceFromAssetFile(filepath);

	if(res == nullptr)
	{
		DecomposedFilePath dfp = App->file_system->DecomposeFilePath(filepath);

		ResourceType type = GetResourceTypeFromAssetExtension(dfp.file_extension_lower_case.c_str());

		if (type != ResourceType::UNKWNOWN)
		{
			res = CreateResource(type);

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

	StartWatchingFolders();

	return ret;
}

bool ModuleResource::ImportAsset(const char * filepath, Resource*& res)
{
	bool ret = false;

	StopWatchingFolders();

	res = GetResourceFromAssetFile(filepath);

	if (res == nullptr)
	{
		DecomposedFilePath dfp = App->file_system->DecomposeFilePath(filepath);

		ResourceType type = GetResourceTypeFromAssetExtension(dfp.file_extension_lower_case.c_str());

		if (type != ResourceType::UNKWNOWN)
		{
			res = CreateResource(type);

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

	StartWatchingFolders();

	return ret;
}

bool ModuleResource::ManageModifiedAsset(const char * filepath)
{
	bool ret = false;

	StopWatchingFolders();

	bool can_load = App->resource->CanLoadFile(filepath);
	bool is_meta = App->resource->IsMeta(filepath);
	bool exists = App->file_system->FileExists(filepath);

	if (!is_meta)
	{
		if (can_load)
		{
			if (exists)
			{
				Resource* loaded_res = nullptr;
				App->resource->ExportAssetToLibrary(filepath);

			}
			else
			{
				UnloadAssetFromEngine(filepath);
			}
		}
		else
		{
			if(exists)
				App->file_system->FileDelete(filepath);
		}
	}
	else
	{
		if (exists)
		{
			std::string asset_filepath = GetAssetFileFromMeta(filepath);

			if (App->file_system->FileExists(asset_filepath.c_str()))
			{
				App->resource->ExportAssetToLibrary(filepath);
			}
			else
				App->file_system->FileDelete(filepath);
		}
		else
		{
			std::string asset_filepath = GetAssetFileFromMeta(filepath);

			App->resource->ExportAssetToLibrary(asset_filepath.c_str());
		}
	}

	StartWatchingFolders();

	return ret;
}

bool ModuleResource::RenameAsset(const char * filepath, const char * new_name)
{
	bool ret = false;

	StopWatchingFolders();

	Resource* res = GetResourceFromAssetFile(filepath);

	if (res != nullptr)
	{
		res->EM_RenameAsset(new_name);
	}

	StartWatchingFolders();

	return ret;
}

bool ModuleResource::CreateScript(const char * filepath, const char * name)
{
	bool ret = false;

	StopWatchingFolders();

	std::string asset_path;
	if (App->scripting->compiler->CreateScriptFromTemplate(filepath, name, asset_path))
	{
		ExportAssetToLibrary(asset_path.c_str());
	}

	StartWatchingFolders();

	return ret;
}

bool ModuleResource::CanLoadFile(const char * filepath)
{
	bool ret = false;

	DecomposedFilePath dfp = App->file_system->DecomposeFilePath(filepath);

	ResourceType type = GetResourceTypeFromAssetExtension(dfp.file_extension_lower_case.c_str());

	if (type != ResourceType::UNKWNOWN)
		ret = true;

	return ret;
}

bool ModuleResource::IsMeta(const char * filepath)
{
	bool ret = false;

	DecomposedFilePath df = App->file_system->DecomposeFilePath(filepath);

	if (df.file_extension_lower_case.compare("meta") == 0)
		ret = true;

	return ret;
}

std::string ModuleResource::GetAssetFileFromMeta(const char * metapath)
{
	DecomposedFilePath df = App->file_system->DecomposeFilePath(metapath);

	std::string asset_path = df.path + df.file_name;

	return asset_path;
}

std::string ModuleResource::GetMetaFileFromAsset(const char * filepath)
{
	std::string ret = filepath + std::string(".meta");
	return ret;
}

void ModuleResource::OnEvent(Event* ev)
{
	switch(ev->GetType())
	{
	case EventType::PROJECT_SELECTED:
		{
			assets_folder = App->file_system->CreateFolder(App->project->GetCurrProjectBasePath().c_str(), "assets");
			library_folder = App->file_system->CreateFolder(App->project->GetCurrProjectBasePath().c_str(), "library");

			current_assets_folder = assets_folder;

			CreateLibraryFolders();

			LoadResourcesTimeSlicedTask* t = new LoadResourcesTimeSlicedTask();
			App->time_sliced->StartTimeSlicedTask(t);

			StartWatchingFolders();

			break;
		}
	case EventType::TIME_SLICED_TASK_FINISHED:
		{

			break;
		}
	case EventType::WATCH_FILE_FOLDER:
		{
			EventWatchFileFolderChanged* efc = (EventWatchFileFolderChanged*)ev;
			
			DecomposedFilePath df = efc->GetPath();

			if(!df.its_folder)
				files_changed_to_check.push_back(df.file_path.c_str());

			break;
		}
	default:
		break;
	}
}


void ModuleResource::StartWatchingFolders()
{
	if (App->project->GetCurrProjectIsSelected())
	{
		--watching_folder_index;

		if (watching_folder_index < 0)
			watching_folder_index = 0;

		if (watching_folder_index == 0)
		{
			App->scripting->file_watcher->WatchFileFolder(GetAssetsPath().c_str());
		}
	}
}

void ModuleResource::StopWatchingFolders()
{
	if (watching_folder_index == 0)
	{
		App->scripting->file_watcher->StopWatchingFileFolder(GetAssetsPath().c_str());
	}

	++watching_folder_index;
}

void ModuleResource::AddAssetExtension(const ResourceType & type, const char * extension)
{
	std::map<ResourceType, std::vector<std::string>>::iterator it = asset_extensions.find(type);

	if (it != asset_extensions.end())
	{
		it->second.push_back(extension);
	}
	else
	{
		std::vector<std::string> new_vec;
		new_vec.push_back(extension);

		asset_extensions[type] = new_vec;
	}
}

void ModuleResource::AddLibraryExtension(const ResourceType & type, const char * extension)
{
	std::map<ResourceType, std::vector<std::string>>::iterator it = library_extensions.find(type);

	if (it != library_extensions.end())
	{
		it->second.push_back(extension);
	}
	else
	{
		std::vector<std::string> new_vec;
		new_vec.push_back(extension);

		library_extensions[type] = new_vec;
	}
}

ResourceType ModuleResource::GetResourceTypeFromAssetExtension(const char * extension)
{
	ResourceType ret = ResourceType::UNKWNOWN;

	for (std::map<ResourceType, std::vector<std::string>>::iterator it = asset_extensions.begin(); it != asset_extensions.end(); ++it)
	{
		ResourceType curr_type = (*it).first;

		std::vector<std::string> extensions = (*it).second;

		for (std::vector<std::string>::iterator ex = extensions.begin(); ex != extensions.end(); ++ex)
		{
			if ((*ex).compare(extension) == 0)
			{
				ret = curr_type;
				break;
			}
		}
	}

	return ret;
}

ResourceType ModuleResource::GetResourceTypeFromLibraryExtension(const char * extension)
{
	ResourceType ret = ResourceType::UNKWNOWN;

	for (std::map<ResourceType, std::vector<std::string>>::iterator it = library_extensions.begin(); it != library_extensions.end(); ++it)
	{
		ResourceType curr_type = (*it).first;

		std::vector<std::string> extensions = (*it).second;

		for (std::vector<std::string>::iterator ex = extensions.begin(); ex != extensions.end(); ++ex)
		{
			if ((*ex).compare(extension) == 0)
			{
				ret = curr_type;
				break;
			}
		}
	}

	return ret;
}

void ModuleResource::AddResourceName(const ResourceType & type, const char * name)
{
	resource_names[type] = name;
}

void ModuleResource::CreateLibraryFolders()
{
	if (App->file_system->FolderExists(library_folder.c_str()))
	{
		for (std::map<ResourceType, std::string>::iterator it = resource_names.begin(); it != resource_names.end(); ++it)
		{
			App->file_system->CreateFolder(library_folder.c_str(), (*it).second.c_str());
		}
	}
}

void ModuleResource::ManageFilesToCheck()
{
	std::vector<std::string> to_check = files_changed_to_check;

	for (std::vector<std::string>::iterator it = to_check.begin(); it != to_check.end(); ++it)
	{
		ManageModifiedAsset((*it).c_str());
	}

	files_changed_to_check.clear();
}

void ModuleResource::DestroyAllResources()
{
}

LoadResourcesTimeSlicedTask::LoadResourcesTimeSlicedTask() : TimeSlicedTask(TimeSlicedTaskType::FOCUS, 4)
{
}

void LoadResourcesTimeSlicedTask::Start()
{
	state = LoadResourcesState::CHECK_ASSET_FILES_IMPORT;

	all_asset_files = App->file_system->GetFilesInPathAndChilds(App->resource->GetAssetsPath().c_str());
	all_library_files = App->file_system->GetFilesInPathAndChilds(App->resource->GetLibraryPath().c_str());

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

		bool is_meta = App->resource->IsMeta(curr_file.c_str());

		if(!is_meta)
		{ 
			bool can_load = App->resource->CanLoadFile(curr_file.c_str());

			if (can_load)
			{
				Resource* loaded_res = nullptr;
				App->resource->ImportAsset(curr_file.c_str(), loaded_res);

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

		App->resource->StopWatchingFolders();

		App->file_system->FileDelete(curr_file.c_str());

		App->resource->StartWatchingFolders();

		files_to_delete.erase(files_to_delete.begin());
	}
	else
	{
		FinishTask();
	}
}
