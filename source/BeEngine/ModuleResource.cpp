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

std::string ModuleResource::GetLibraryPath()
{
	return library_folder;
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
			library_folder = App->file_system->CreateFolder(App->project->GetCurrProjectBasePath().c_str(), "library");

			CreateLibraryPaths();
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
			ret = ImportAssetToEngine(filepath, resources);

			if(ret)
				ret = ExportResourcesToLibrary(resources);
		}
	}

	return ret;
}

bool ModuleResource::UnloadAssetFromEngine(const char * filepath)
{
	bool ret = false;

	ret = ClearAssetDataFromEngine(filepath);

	if(ret)
		App->file_system->FileDelete(filepath);

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

bool ModuleResource::IsMetaFile(const char * filepath, const char * metapath)
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
