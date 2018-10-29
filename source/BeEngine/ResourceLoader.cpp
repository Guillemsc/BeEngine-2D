#include "Globals.h"
#include "ResourceLoader.h"
#include "Functions.h"

ResourceLoader::ResourceLoader(ResourceType to_load_type)
{
	resources_to_load_type = to_load_type;
}

ResourceType ResourceLoader::GetResourcesToLoadType() const
{
	return resources_to_load_type;
}

void ResourceLoader::AddAssetExtensionToLoad(const char * extension)
{
	std::string to_add = ToLowerCase(extension);
	assets_extensions_to_load.push_back(to_add);
}

void ResourceLoader::AddLibraryExtensionToLoad(const char * extension)
{
	std::string to_add = ToLowerCase(extension);
	library_extensions_to_load.push_back(to_add);
}

bool ResourceLoader::CanLoadExtensionAsset(const char * extension) 
{
	bool ret = false;

	std::string to_check = ToLowerCase(extension);

	for (std::vector<std::string>::iterator it = assets_extensions_to_load.begin(); it != assets_extensions_to_load.end(); ++it)
	{
		if (TextCmp(to_check.c_str(), (*it).c_str()))
		{
			ret = true;
			break;
		}
	}

	return ret;
}

bool ResourceLoader::CanLoadExtensionLibrary(const char * extension) 
{
	bool ret = false;

	std::string to_check = ToLowerCase(extension);

	for (std::vector<std::string>::iterator it = library_extensions_to_load.begin(); it != library_extensions_to_load.end(); ++it)
	{
		if (TextCmp(to_check.c_str(), (*it).c_str()))
		{
			ret = true;
			break;
		}
	}

	return ret;
}

void ResourceLoader::AddResource(Resource * resource)
{
	resources[resource->GetUID()] = resource;
}

bool ResourceLoader::DestroyResource(std::string unique_id)
{
	bool ret = false;

	std::map<std::string, Resource*>::iterator it = resources.find(unique_id);

	if (it != resources.end())
	{
		OnDestroyResource((*it).second);

		(*it).second->CleanUp();
		RELEASE(it->second);

		resources.erase(it);

		ret = true;
	}

	return ret;
}

bool ResourceLoader::DestroyResource(Resource* res)
{
	bool ret = false;

	for (std::map<std::string, Resource*>::iterator it = resources.begin(); it != resources.end(); ++it)
	{
		if (res == (*it).second)
		{
			OnDestroyResource((*it).second);

			res->CleanUp();
			RELEASE(res);

			resources.erase(it);

			ret = true;

			break;
		}
	}

	return ret;
}

void ResourceLoader::DestroyAllResources()
{
	for (std::map<std::string, Resource*>::iterator it = resources.begin(); it != resources.end(); ++it)
	{		
		OnDestroyResource((*it).second);

		(*it).second->CleanUp();
		RELEASE((*it).second);
	}

	resources.clear();
}

Resource * ResourceLoader::GetResource(std::string unique_id)
{
	std::map<std::string, Resource*>::iterator it = resources.find(unique_id);

	if (it != resources.end())
		return it->second;

	return nullptr;
}

std::map<std::string, Resource*> ResourceLoader::GetResources() const
{
	return resources;
}

std::string ResourceLoader::GetLibraryPath()
{
	return library_path;
}
