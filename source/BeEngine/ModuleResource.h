#ifndef __MODULE_RESOURCE_H__
#define __MODULE_RESOURCE_H__

#include <map>

#include "Module.h"
#include "Resource.h"
#include "ResourceLoader.h"

class ResourceTextureLoader;
class Event;

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

	std::string GetLibraryPath();

	ResourceType AssetExtensionToType(const char* extension);
	ResourceType LibraryExtensionToType(const char* extension);

	Resource* CreateResource(ResourceType type, std::string unique_id);
	Resource* CreateResource(ResourceType type);

	Resource* GetResource(std::string unique_id, ResourceType type);
	Resource* GetResource(std::string unique_id);

	bool DestroyResource(std::string unique_id, ResourceType type);
	bool DestroyResource(std::string unique_id);
	bool DestroyResource(Resource* res);

	bool LoadFileToEngine(const char* filepath, std::vector<Resource*>& resources = std::vector<Resource*>());
	bool UnloadAssetFromEngine(const char* filepath);
	bool ClearAssetDataFromEngine(const char* filepath);
	bool ImportAssetToEngine(const char* filepath, std::vector<Resource*>& resources = std::vector<Resource*>());
	bool ReimportAssetToEngine(const char* filepath);
	bool RenameAsset(const char* filepath, const char* new_name);
	bool IsMetaFile(const char* filepath, const char* metapath);
	bool IsAssetOnLibrary(const char* filepath, std::vector<std::string>& library_files_used = std::vector<std::string>());
	bool ImportResourceFromLibrary(const char* filepath);
	bool ExportResourceToLibrary(Resource* resource);
	bool ExportResourcesToLibrary(const std::vector<Resource*>& resources);

private:
	ResourceLoader* AddLoader(ResourceLoader* loader);
	void DestroyAllLoaders();
	ResourceLoader* GetLoader(ResourceType type);

	void CreateLibraryPaths();

	void DestroyAllResources();

	void OnEvent(Event* ev);

public:
	ResourceTextureLoader* texture_loader = nullptr;

private:
	std::map<ResourceType, ResourceLoader*> loaders;

	std::string current_assets_folder;
	std::string library_folder;
};

#endif // !__MODULE_RESOURCE_H__