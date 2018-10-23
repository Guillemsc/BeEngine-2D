#include "ModuleResource.h"
#include "Functions.h"

ModuleResource::ModuleResource()
{
}

ModuleResource::~ModuleResource()
{
}

bool ModuleResource::Awake()
{
	bool ret = true;

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

ResourceLoader * ModuleResource::AddLoader(ResourceLoader * loader)
{
	ResourceLoader* ret = loader;

	loaders[loader->GetResourcesToLoadType()] = loader;

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

void ModuleResource::DestroyAllResources()
{
	for (std::map<ResourceType, ResourceLoader*>::iterator it = loaders.begin(); it != loaders.end(); ++it)
	{
		(*it).second->DestroyAllResources();
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
