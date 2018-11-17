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

	AddLibraryName(ResourceType::TEXTURE, "textures");

	AddAssetExtension(ResourceType::TEXTURE, "png");
	AddAssetExtension(ResourceType::TEXTURE, "jpg");

	//texture_loader = (ResourceTextureLoader*)AddLoader(new ResourceTextureLoader());
	//texture_loader->AddAssetExtensionToLoad("png");
	//texture_loader->AddAssetExtensionToLoad("jpg");

	//script_loader = (ResourceScriptLoader*)AddLoader(new ResourceScriptLoader());
	//script_loader->AddAssetExtensionToLoad("cs");

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

std::string ModuleResource::GetLibraryPathFromResourceType(const ResourceType & type)
{
	std::string ret = "";

	ret += library_folder;

	ret += library_names[type] + "\\";

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
	default:
		break;
	}

	if (ret != nullptr)
	{
		resources.push_back(ret);
	}

	return ret;
}

Resource* ModuleResource::GetResourceFromAssetFile(const char* filepath)
{
	Resource* ret = nullptr;

	for (std::vector<Resource*>::iterator it = resources.begin(); it != resources.end(); ++it)
	{
		if ((*it)->asset_filepath.compare(filepath) == 0)
		{
			ret = (*it);
			break;
		}
	}

	return ret;
}

bool ModuleResource::LoadFileToEngine(const char * filepath)
{
	bool ret = false;

	if (App->file_system->FileExists(filepath))
	{
		std::string new_path;
		bool can_load = App->file_system->FileCopyPaste(filepath, current_assets_folder.c_str(), false, new_path);

		if (can_load)
		{
			ret = ExportAssetToLibrary(new_path.c_str());
		}
	}

	return ret;
}

bool ModuleResource::ExportAssetToLibrary(const char * filepath)
{
	bool ret = false;

	Resource* res = GetResourceFromAssetFile(filepath);

	if(res == nullptr)
	{
		DecomposedFilePath dfp = App->file_system->DecomposeFilePath(filepath);

		ResourceType type = GetResourceTypeFromAssetExtension(dfp.file_extension_lower_case.c_str());

		res = CreateResource(type);

		if(res != nullptr)
			res->EM_InitResource(filepath);
	}
	else
	{
		res->EM_ExportToLibrary();
	}

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

			LoadFileToEngine("C:\\Users\\Guillem\\Desktop\\Files\\sans.png");

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
			App->scripting->file_watcher->WatchFileFolder(GetLibraryPath().c_str());
		}
	}
}

void ModuleResource::StopWatchingFolders()
{
	if (watching_folder_index == 0)
	{
		App->scripting->file_watcher->StopWatchingFileFolder(GetAssetsPath().c_str());
		App->scripting->file_watcher->StopWatchingFileFolder(GetLibraryPath().c_str());
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

void ModuleResource::AddLibraryName(const ResourceType & type, const char * name)
{
	library_names[type] = name;
}

void ModuleResource::CreateLibraryFolders()
{
	if (App->file_system->FolderExists(library_folder.c_str()))
	{
		for (std::map<ResourceType, std::string>::iterator it = library_names.begin(); it != library_names.end(); ++it)
		{
			App->file_system->CreateFolder(library_folder.c_str(), (*it).second.c_str());
		}
	}
}

void ModuleResource::DestroyAllResources()
{
}


