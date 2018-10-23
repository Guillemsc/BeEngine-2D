#ifndef __MODULE_RESOURCE_H__
#define __MODULE_RESOURCE_H__

#include <map>

#include "Module.h"
#include "Resource.h"
#include "ResourceLoader.h"

class ModuleResource: public Module
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

	ResourceType AssetExtensionToType(const char* extension);
	ResourceType LibraryExtensionToType(const char* extension);

	Resource* CreateResource(ResourceType type, std::string unique_id);
	Resource* CreateResource(ResourceType type);

	Resource* GetResource(std::string unique_id, ResourceType type);
	Resource* GetResource(std::string unique_id);

	bool DestroyResource(std::string unique_id, ResourceType type);
	bool DestroyResource(std::string unique_id);
	bool DestroyResource(Resource* res);

	void LoadFileToEngine(const char* filepath, std::vector<Resource*>& resources = std::vector<Resource*>());
	void UnloadAssetFromEngine(const char* filepath);
	void ClearAssetDataFromEngine(const char* filepath);
	void ReimportAssetToEngine(const char* filepath);
	void RenameAsset(const char* filepath, const char* new_name);
	bool IsAssetOnLibrary(const char* filepath, std::vector<std::string>& library_files_used = std::vector<std::string>());
	void ImportResourceFromLibrary(const char* uid);
	void ExportResourceToLibrary(Resource* resource);

private:
	ResourceLoader* AddLoader(ResourceLoader* loader);
	void DestroyAllLoaders();
	ResourceLoader* GetLoader(ResourceType type);

	void DestroyAllResources();

private:
	std::map<ResourceType, ResourceLoader*> loaders;

};

#endif // !__MODULE_RESOURCE_H__