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
#include "ResourcePrefab.h"
#include "ResourceScene.h"
#include "ScriptingObjectCompiler.h"
#include "ModuleInput.h"
#include "ModuleScripting.h"
#include "ScriptingObjectSolutionManager.h"
#include "Event.h"
#include "ModuleAssets.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")

ModuleResource::ModuleResource() : Module()
{

}

ModuleResource::~ModuleResource()
{
	
}

bool ModuleResource::Awake()
{
	bool ret = true;

	AddResourceName(ResourceType::RESOURCE_TYPE_TEXTURE, "texture");
	AddResourceName(ResourceType::RESOURCE_TYPE_SCRIPT, "script");
	AddResourceName(ResourceType::RESOURCE_TYPE_PREFAB, "prefab");
	AddResourceName(ResourceType::RESOURCE_TYPE_SCENE, "scene");

	AddAssetExtension(ResourceType::RESOURCE_TYPE_TEXTURE, "png");
	AddAssetExtension(ResourceType::RESOURCE_TYPE_TEXTURE, "jpg");
	AddLibraryExtension(ResourceType::RESOURCE_TYPE_TEXTURE, "dds");

	AddAssetExtension(ResourceType::RESOURCE_TYPE_SCRIPT, "cs");

	AddAssetExtension(ResourceType::RESOURCE_TYPE_PREFAB, "prefab");
	AddLibraryExtension(ResourceType::RESOURCE_TYPE_PREFAB, "prefab");

	AddAssetExtension(ResourceType::RESOURCE_TYPE_SCENE, "scene");
	AddLibraryExtension(ResourceType::RESOURCE_TYPE_SCENE, "scene");

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

std::string ModuleResource::GetResourceNameFromResourceType(const ResourceType & type)
{
	return resource_names[type];
}

std::string ModuleResource::GetLibraryPathFromResourceType(const ResourceType & type)
{
	std::string ret = "";

	ret += App->assets->GetLibraryPath();

	ret += resource_names[type] + "\\";

	return ret;
}

Resource* ModuleResource::CreateResource(const ResourceType type)
{
	Resource* ret = nullptr;

	switch (type)
	{
	case ResourceType::RESOURCE_TYPE_TEXTURE:

		ret = new ResourceTexture();
		
		break;

	case ResourceType::RESOURCE_TYPE_SCRIPT:

		ret = new ResourceScript();

		break;

	case ResourceType::RESOURCE_TYPE_PREFAB:

		ret = new ResourcePrefab();

		break;

	case ResourceType::RESOURCE_TYPE_SCENE:

		ret = new ResourceScene();

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
				EventResourceDestroyed* event_res_destroyed = new EventResourceDestroyed(res);
				App->event->SendEvent(event_res_destroyed);

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

Resource * ModuleResource::GetResourceFromUid(std::string uid, ResourceType type)
{
	Resource* ret = nullptr;

	if (type != ResourceType::RESOURCE_TYPE_UNKWNOWN)
	{
		std::vector<Resource*> to_check = resources[type];

		for (std::vector<Resource*>::iterator it = to_check.begin(); it != to_check.end(); ++it)
		{
			if ((*it)->uid.compare(uid) == 0)
			{
				ret = (*it);
				break;
			}
		}
	}

	if (ret == nullptr)
	{
		for (std::map<ResourceType, std::vector<Resource*>>::iterator it = resources.begin(); it != resources.end(); ++it)
		{
			if (type != (*it).first)
			{
				for (std::vector<Resource*>::iterator t = (*it).second.begin(); t != (*it).second.end(); ++t)
				{
					if ((*t)->uid.compare(uid) == 0)
					{
						ret = (*t);
						break;
					}
				}
			}
		}
	}

	return ret;
}

std::vector<Resource*> ModuleResource::GetResourcesFromResourceType(const ResourceType type)
{
	std::vector<Resource*> ret;

	for (std::map<ResourceType, std::vector<Resource*>>::iterator it = resources.begin(); it != resources.end(); ++it)
	{
		if ((*it).first == type)
		{
			ret = (*it).second;
			break;
		}
	}

	return ret;
}

std::map<ResourceType, std::vector<Resource*>> ModuleResource::GetAllResources() const
{
	return resources;
}

std::map<ResourceType, std::string> ModuleResource::GetAllResourcesNames() const
{
	return resource_names;
}

std::vector<Resource*> ModuleResource::GetResourcesOnAssetsPath(const std::string& path)
{
	std::vector<Resource*> ret;

	for (std::map<ResourceType, std::vector<Resource*>>::iterator it = resources.begin(); it != resources.end(); ++it)
	{
		for (std::vector<Resource*>::iterator t = (*it).second.begin(); t != (*it).second.end(); ++t)
		{
			if ((*t)->d_asset_filepath.path.compare(path) == 0)
			{
				ret.push_back(*t);
			}
		}
	}

	return ret;
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
	ResourceType ret = ResourceType::RESOURCE_TYPE_UNKWNOWN;

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
	ResourceType ret = ResourceType::RESOURCE_TYPE_UNKWNOWN;

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

void ModuleResource::DestroyAllResources()
{
	for (std::map<ResourceType, std::vector<Resource*>>::iterator it = resources.begin(); it != resources.end(); ++it)
	{
		for (std::vector<Resource*>::iterator t = (*it).second.begin(); t != (*it).second.end(); ++t)
		{			
			EventResourceDestroyed* event_res_destroyed = new EventResourceDestroyed(*t);
			App->event->SendEvent(event_res_destroyed);

			(*t)->CleanUp();

			RELEASE(*t);
		}

		(*it).second.clear();
	}

	resources.clear();
}
