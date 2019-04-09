#ifndef __MODULE_RESOURCE_H__
#define __MODULE_RESOURCE_H__

#include <map>

#include "Module.h"
#include "Resource.h"
#include "ModuleTimeSlicedTask.h"

class Event;
class LoadResourcesTimeSlicedTask;
class ManageModifiedAssetsTimeSlicedTask;
class ResourceScript;

class ModuleResource : public Module
{
public:
	ModuleResource();
	~ModuleResource();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

	std::string GetNewUID();

	std::string GetResourceNameFromResourceType(const ResourceType& type);
	std::string GetLibraryPathFromResourceType(const ResourceType& type);

	Resource* CreateResource(const ResourceType type);
	void DestroyResource(Resource* res);
	Resource* GetResourceFromAssetFile(const char* filepath);
	Resource* GetResourceFromUid(std::string uid, ResourceType type = ResourceType::RESOURCE_TYPE_UNKWNOWN);
	std::vector<Resource*> GetResourcesFromResourceType(const ResourceType type);
	ResourceScript* GetResourceScriptFromScriptName(const std::string& name);
	std::map<ResourceType, std::vector<Resource*>> GetAllResources() const;
	std::map<ResourceType, std::string> GetAllResourcesNames() const;
	std::vector<Resource*> GetResourcesOnAssetsPath(const std::string& path);

	ResourceType GetResourceTypeFromAssetExtension(const char* extension);
	ResourceType GetResourceTypeFromLibraryExtension(const char* extension);

private:
	void AddAssetExtension(const ResourceType& type, const char* extension);
	void AddLibraryExtension(const ResourceType& type, const char* extension);

	void AddResourceName(const ResourceType& type, const char* name);

	void DestroyAllResources();

private:
	std::map<ResourceType, std::vector<std::string>> asset_extensions;
	std::map<ResourceType, std::vector<std::string>> library_extensions;
	std::map<ResourceType, std::string> resource_names;

	std::map<ResourceType, std::vector<Resource*>> resources;
};


#endif // !__MODULE_RESOURCE_H__