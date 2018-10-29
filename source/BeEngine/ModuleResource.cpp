#include "ModuleResource.h"
#include "Functions.h"
#include "App.h"
#include "ModuleFileSystem.h"
#include "ResourceTextureLoader.h"
#include "ModuleEvent.h"
#include "ModuleProject.h"

ModuleResource::ModuleResource()
{

}

ModuleResource::~ModuleResource()
{
}

bool ModuleResource::Awake()
{
	bool ret = true;

	App->event->Suscribe(std::bind(&ModuleResource::OnEvent, this, std::placeholders::_1), EventType::PROJECT_SELECTED);

	texture_loader = (ResourceTextureLoader*)AddLoader(new ResourceTextureLoader());

	texture_loader->AddAssetExtensionToLoad("png");
	texture_loader->AddAssetExtensionToLoad("jpg");

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
	DestroyAllLoaders();

	return ret;
}

std::string ModuleResource::GetNewUID()
{	
	return GetUIDRandomHexadecimal();
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

ResourceLoader * ModuleResource::AddLoader(ResourceLoader * loader)
{
	ResourceLoader* ret = loader;

	if (loader != nullptr)
	{
		loaders[loader->GetResourcesToLoadType()] = loader;

		loader->Start();
	}

	return ret;
}

void ModuleResource::DestroyAllLoaders()
{
	for (std::map<ResourceType, ResourceLoader*>::iterator it = loaders.begin(); it != loaders.end(); ++it)
	{
		(*it).second->CleanUp();
		RELEASE((*it).second);
	}

	loaders.clear();
}

ResourceLoader* ModuleResource::GetLoader(ResourceType type)
{
	return loaders[type];
}

void ModuleResource::CreateLibraryPaths()
{
	for (std::map<ResourceType, ResourceLoader*>::iterator it = loaders.begin(); it != loaders.end(); ++it)
	{
		(*it).second->CreateLibraryFolder();
	}
}

void ModuleResource::DestroyAllResources()
{
	for (std::map<ResourceType, ResourceLoader*>::iterator it = loaders.begin(); it != loaders.end(); ++it)
	{
		(*it).second->DestroyAllResources();
	}
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

			CreateLibraryPaths();

			LoadFileToEngine("C:\\Users\\Guillem\\Desktop\\Files\\37311279_p0.jpg");

			break;
		}
	default:
		break;
	}
}

ResourceType ModuleResource::AssetExtensionToType(const char * extension)
{
	ResourceType ret;

	for (std::map<ResourceType, ResourceLoader*>::iterator it = loaders.begin(); it != loaders.end(); ++it)
	{
		if ((*it).second->CanLoadExtensionAsset(extension))
		{
			ret = (*it).first;
			break;
		}
	}

	return ret;
}

ResourceType ModuleResource::LibraryExtensionToType(const char * extension)
{
	ResourceType ret;

	for (std::map<ResourceType, ResourceLoader*>::iterator it = loaders.begin(); it != loaders.end(); ++it)
	{
		if ((*it).second->CanLoadExtensionLibrary(extension))
		{
			ret = (*it).first;
			break;
		}
	}

	return ret;
}

Resource* ModuleResource::CreateResource(ResourceType type, std::string unique_id)
{
	Resource* res = nullptr;

	res = GetResource(unique_id);

	if (res == nullptr)
	{
		ResourceLoader* loader = GetLoader(type);

		if (loader != nullptr)
		{
			res = loader->CreateResource(unique_id);

			loader->AddResource(res);
		}
	}

	return res;
}

Resource* ModuleResource::CreateResource(ResourceType type)
{
	std::string new_id = GetNewUID();

	return CreateResource(type, new_id);
}

Resource * ModuleResource::GetResource(std::string unique_id, ResourceType type)
{
	Resource* ret = nullptr;

	ResourceLoader* loader = GetLoader(type);

	if (loader != nullptr)
	{
		ret = loader->GetResource(unique_id);
	}

	return ret;
}

Resource * ModuleResource::GetResource(std::string unique_id)
{
	Resource* ret = nullptr;

	for (std::map<ResourceType, ResourceLoader*>::iterator it = loaders.begin(); it != loaders.end(); ++it)
	{
		ret = (*it).second->GetResource(unique_id);

		if (ret != nullptr)
		{
			break;
		}
	}

	return ret;
}

bool ModuleResource::DestroyResource(std::string unique_id, ResourceType type)
{
	bool ret = false;

	ResourceLoader* loader = GetLoader(type);

	if(loader != nullptr)
	{
		ret = loader->DestroyResource(unique_id);
	}

	return ret;
}

bool ModuleResource::DestroyResource(std::string unique_id)
{
	bool ret = false;

	for (std::map<ResourceType, ResourceLoader*>::iterator it = loaders.begin(); it != loaders.end(); ++it)
	{
		ResourceLoader* loader = (*it).second;

		if (loader->DestroyResource(unique_id))
		{
			ret = true;
			break;
		}
	}

	return ret;
}

bool ModuleResource::DestroyResource(Resource* res)
{
	bool ret = false;

	if (res != nullptr)
	{
		ResourceLoader* loader = GetLoader(res->GetType());

		if (loader != nullptr)
		{
			ret = loader->DestroyResource(res);
		}
	}

	return ret;
}

bool ModuleResource::LoadFileToEngine(const char * filepath, std::vector<Resource*>& resources)
{
	bool ret = false;

	if (App->file_system->FileExists(filepath))
	{
		std::string new_path;
		bool can_load = App->file_system->FileCopyPaste(filepath, current_assets_folder.c_str(), false, new_path);

		if (can_load)
		{
			ret = ImportAssetToEngine(new_path.c_str(), resources);

			if(ret)
				ret = ExportResourcesToLibrary(resources);
		}
	}

	return ret;
}

bool ModuleResource::UnloadAssetFromEngine(const char * filepath)
{
	bool ret = false;

	if (App->file_system->FileExists(filepath))
	{
		ClearAssetDataFromEngine(filepath);

		DeleteAssetResources(filepath);

		App->file_system->FileDelete(filepath);

		ret = true;
	}

	return ret;
}

bool ModuleResource::DeleteAssetResources(const char * filepath)
{
	bool ret = false;

	if (App->file_system->FileExists(filepath))
	{
		DecomposedFilePath d_filepath = App->file_system->DecomposeFilePath(filepath);

		ResourceType type = AssetExtensionToType(d_filepath.file_extension.c_str());
		ResourceLoader* loader = GetLoader(type);

		if (loader != nullptr)
		{
			ret = loader->DeleteAssetResources(d_filepath);
		}
	}

	return ret;
}

bool ModuleResource::ClearAssetDataFromEngine(const char * filepath)
{
	bool ret = false;

	if (App->file_system->FileExists(filepath))
	{
		DecomposedFilePath d_filepath = App->file_system->DecomposeFilePath(filepath);

		ResourceType type = AssetExtensionToType(d_filepath.file_extension.c_str());
		ResourceLoader* loader = GetLoader(type);

		if (loader != nullptr)
		{
			ret = loader->ClearAssetDataFromEngine(d_filepath);
		}
	}

	return ret;
}

bool ModuleResource::ImportAssetToEngine(const char * filepath, std::vector<Resource*>& resources)
{
	bool ret = false;

	if (App->file_system->FileExists(filepath))
	{
		DecomposedFilePath deco_file = App->file_system->DecomposeFilePath(filepath);

		ResourceType type = AssetExtensionToType(deco_file.file_extension.c_str());

		ResourceLoader* loader = GetLoader(type);

		if (loader != nullptr)
		{
			ret = loader->ImportAssetToEngine(deco_file, resources);
		}
	}

	return ret;
}

bool ModuleResource::ReimportAssetToEngine(const char * filepath)
{
	bool ret = false;

	if (App->file_system->FileExists(filepath))
	{
		DeleteAssetResources(filepath);
		ClearAssetDataFromEngine(filepath);

		ret = ImportAssetToEngine(filepath);
	}

	return ret;
}

bool ModuleResource::RenameAsset(const char * filepath, const char * new_name)
{
	bool ret = false;

	DecomposedFilePath d_filepath = App->file_system->DecomposeFilePath(filepath);

	ResourceType type = AssetExtensionToType(d_filepath.file_extension.c_str());

	ResourceLoader* loader = GetLoader(type);

	if (loader != nullptr)
	{
		ret = loader->RenameAsset(d_filepath, new_name);
	}

	return ret;
}

bool ModuleResource::IsMetaOfFile(const char * filepath, const char * metapath)
{
	bool ret = false;

	if (App->file_system->FileExists(filepath))
	{
		if (App->file_system->FileExists(metapath))
		{
			std::string filepath_meta = std::string(filepath) + ".meta";

			if (filepath_meta == std::string(metapath))
			{
				ret = true;
			}
		}
	}

	return ret;
}

bool ModuleResource::IsMeta(const char * filepath)
{
	bool ret = false;

	if (App->file_system->FileExists(filepath))
	{
		DecomposedFilePath deco_file = App->file_system->DecomposeFilePath(filepath);

		if (deco_file.file_extension == "meta")
			ret = true;
	}

	return ret;
}

bool ModuleResource::IsAssetOnLibrary(const char * filepath, std::vector<std::string>& library_files_used)
{
	bool ret = false;

	DecomposedFilePath deco_file = App->file_system->DecomposeFilePath(filepath);

	ResourceType type = AssetExtensionToType(deco_file.file_extension.c_str());

	ResourceLoader* loader = GetLoader(type);

	if (loader != nullptr)
	{
		ret = loader->IsAssetOnLibrary(deco_file, library_files_used);
	}

	return ret;
}

bool ModuleResource::ImportResourceFromLibrary(const char * filepath)
{
	bool ret = false;

	DecomposedFilePath deco_file = App->file_system->DecomposeFilePath(filepath);

	ResourceType type = LibraryExtensionToType(deco_file.file_extension.c_str());

	ResourceLoader* loader = GetLoader(type);

	if (loader != nullptr)
	{
		ret = loader->ImportResourceFromLibrary(deco_file);
	}

	return ret;
}

bool ModuleResource::ExportResourceToLibrary(Resource * resource)
{
	bool ret = false;

	if (resource != nullptr)
	{
		if (resource != nullptr)
		{
			ResourceLoader* loader = GetLoader(resource->GetType());

			if (loader != nullptr)
			{
				ret = loader->ExportResourceToLibrary(resource);
			}
		}
	}

	return ret;
}

bool ModuleResource::ExportResourcesToLibrary(const std::vector<Resource*>& resources)
{
	bool ret = false;

	for (std::vector<Resource*>::const_iterator it = resources.begin(); it != resources.end(); ++it)
	{
		ret = ExportResourceToLibrary((*it));

		if (!ret)
			break;
	}

	return ret;
}

CheckAssetsErrorsTimeSlicedTask::CheckAssetsErrorsTimeSlicedTask(ModuleResource * module_proj) : TimeSlicedTask(TimeSlicedTaskType::FOCUS)
{
	
}

void CheckAssetsErrorsTimeSlicedTask::Start()
{
	state = CheckErrorsState::CHECK_ASSET_FILES;

	all_asset_files = App->file_system->GetFilesInPathAndChilds(App->resource->GetAssetsPath().c_str());

	asset_files_to_check = all_asset_files;
}

void CheckAssetsErrorsTimeSlicedTask::Update()
{
	switch (state)
	{
	case CheckAssetsErrorsTimeSlicedTask::CHECK_ASSET_FILES:
		CheckAssetFiles();
		break;
	case CheckAssetsErrorsTimeSlicedTask::CHECK_ASSET_META_FILES:
		CheckAssetMetaFiles();
		break;
	case CheckAssetsErrorsTimeSlicedTask::DELETE_UNNECESSARY_FILES:
		DeleteUnnecessaryFiles();
		break;
	case CheckAssetsErrorsTimeSlicedTask::REIMPORT_ASSETS:
		ReimportAssets();
		break;
	default:
		break;
	}
}

void CheckAssetsErrorsTimeSlicedTask::Finish()
{
}

void CheckAssetsErrorsTimeSlicedTask::CheckAssetFiles()
{
	if (!asset_files_to_check.empty())
	{
		float progress = 100 - ((float)all_asset_files.size() / 100.0f) * asset_files_to_check.size();
		SetPercentageProgress(progress);
		SetDescription("Checking asset files");

		std::string curr_file = *asset_files_to_check.begin();

		bool is_meta = App->resource->IsMeta(curr_file.c_str());

		if (is_meta)
		{
			asset_metas_to_check.push_back(curr_file);
			all_asset_metas_to_check_count = asset_metas_to_check.size();
		}
		else
		{
			std::vector<std::string> library_files_to_check;
			bool correct = App->resource->IsAssetOnLibrary(curr_file.c_str(), library_files_to_check);

			if (correct)
			{
				library_files_used.insert(library_files_used.end(), library_files_to_check.begin(), library_files_to_check.end());
			}
			else
			{
				assets_to_reimport.push_back(curr_file);
				all_assets_to_reimport_count = assets_to_reimport.size();
			}
		}

		asset_files_to_check.erase(asset_files_to_check.begin());
	}
	else
	{
		asset_files_to_check = all_asset_files;

		state = CheckErrorsState::CHECK_ASSET_META_FILES;
	}
}

void CheckAssetsErrorsTimeSlicedTask::CheckAssetMetaFiles()
{
	if (!asset_metas_to_check.empty())
	{
		float progress = 100 - ((float)all_asset_metas_to_check_count / 100.0f) * asset_metas_to_check.size();
		SetPercentageProgress(progress);
		SetDescription("Checking asset meta files");

		std::string curr_file = *asset_metas_to_check.begin();

		bool used = false;
		for (std::vector<std::string>::iterator it = asset_files_to_check.begin(); it != asset_files_to_check.end();)
		{
			bool is_meta = App->resource->IsMeta((*it).c_str());

			if (!is_meta)
			{
				if (App->resource->IsMetaOfFile((*it).c_str(), curr_file.c_str()))
				{
					used = true;
					it = asset_files_to_check.erase(it);
					break;
				}
				else
					++it;
			}
			else
			{
				it = asset_files_to_check.erase(it);
			}
		}

		if (!used)
		{
			App->file_system->FileDelete(curr_file.c_str());
		}

		asset_metas_to_check.erase(asset_metas_to_check.begin());
	}
	else
	{
		library_files_to_check = App->file_system->GetFilesInPathAndChilds(App->resource->GetLibraryPath().c_str());
		all_library_files_to_check_count = library_files_to_check.size();

		state = CheckErrorsState::DELETE_UNNECESSARY_FILES;
	}
}

void CheckAssetsErrorsTimeSlicedTask::DeleteUnnecessaryFiles()
{
	if (!library_files_to_check.empty())
	{
		float progress = 100 - ((float)all_library_files_to_check_count / 100.0f) * library_files_to_check.size();
		SetPercentageProgress(progress);
		SetDescription("Deleting library garbage");

		std::string curr_file = *library_files_to_check.begin();

		bool to_delete = true;
		for (std::vector<std::string>::iterator it = library_files_used.begin(); it != library_files_used.end(); ++it)
		{
			if ((*it) == curr_file)
			{
				library_files_used.erase(it);
				to_delete = false;
				break;
			}
		}

		if (to_delete)
		{
			App->file_system->FileDelete(curr_file.c_str());
		}

		library_files_to_check.erase(library_files_to_check.begin());
	}
	else
	{
		state = CheckErrorsState::REIMPORT_ASSETS;
	}
}

void CheckAssetsErrorsTimeSlicedTask::ReimportAssets()
{
	if (!assets_to_reimport.empty())
	{
		float progress = 100 - ((float)all_assets_to_reimport_count / 100.0f) * assets_to_reimport.size();
		SetPercentageProgress(progress);
		SetDescription("Reimporting assets");

		std::string curr_file = *assets_to_reimport.begin();

		App->resource->ImportAssetToEngine(curr_file.c_str());

		assets_to_reimport.erase(assets_to_reimport.begin());
	}
	else
	{
		SetPercentageProgress(100);

		FinishTask();
	}
}
