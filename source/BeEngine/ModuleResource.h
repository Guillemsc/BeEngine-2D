#ifndef __MODULE_RESOURCE_H__
#define __MODULE_RESOURCE_H__

#include <map>

#include "Module.h"
#include "Resource.h"
#include "ResourceLoader.h"
#include "ModuleTimeSlicedTask.h"

class ResourceTextureLoader;
class Event;

class ModuleResource : public Module
{
	friend class CheckAssetsErrorsTimeSlicedTask;

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

	std::string GetCurrentAssetsPath();
	std::string GetLibraryPath();
	std::string GetAssetsPath();

	ResourceType AssetExtensionToType(const char* extension);
	ResourceType LibraryExtensionToType(const char* extension);

	Resource* CreateResource(ResourceType type, std::string unique_id);
	Resource* CreateResource(ResourceType type);

	Resource* GetResource(std::string unique_id, ResourceType type);
	Resource* GetResource(std::string unique_id);

	bool DestroyResource(std::string unique_id, ResourceType type);
	bool DestroyResource(std::string unique_id);
	bool DestroyResource(Resource* res);

public:
	bool LoadFileToEngine(const char* filepath, std::vector<Resource*>& resources = std::vector<Resource*>());
	bool UnloadAssetFromEngine(const char* filepath);
	bool ReimportAssetToEngine(const char* filepath);
	bool RenameAsset(const char* filepath, const char* new_name);
	bool ExportResourceToLibrary(Resource* resource);

private:
	bool DeleteAssetResources(const char* filepath);
	bool ClearAssetDataFromEngine(const char* filepath);
	bool ExportAssetToLibrary(const char* filepath);
	bool IsAssetOnLibrary(const char* filepath, std::vector<std::string>& library_files_used = std::vector<std::string>());
	bool ImportAssetFromLibrary(const char* filepath, std::vector<Resource*>& resources = std::vector<Resource*>());
	bool IsMetaOfFile(const char* filepath, const char* metapath);
	bool IsMeta(const char * filepath);

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
	std::string assets_folder;
	std::string library_folder;
};

class CheckAssetsErrorsTimeSlicedTask : public TimeSlicedTask
{
	enum CheckErrorsState
	{
		CHECK_ASSET_FILES,
		CHECK_ASSET_META_FILES,
		DELETE_UNNECESSARY_FILES,
		REIMPORT_ASSETS,
	};

public:
	CheckAssetsErrorsTimeSlicedTask(ModuleResource* module_proj);

	void Start();
	void Update();
	void Finish();

private:
	void CheckAssetFiles();
	void CheckAssetMetaFiles();
	void DeleteUnnecessaryFiles();
	void ReimportAssets();

private:
	ModuleResource* module_proj = nullptr;

	std::vector<std::string> all_asset_files;

	std::vector<std::string> asset_files_to_check;

	std::vector<std::string> asset_metas_to_check;
	int all_asset_metas_to_check_count = 0;

	std::vector<std::string> library_files_to_check;
	int all_library_files_to_check_count = 0;

	std::vector<std::string> library_files_used;
	int all_library_files_used_count = 0;

	std::vector<std::string> assets_to_reimport;
	int all_assets_to_reimport_count = 0;

	CheckErrorsState state = CheckErrorsState::CHECK_ASSET_FILES;
};

#endif // !__MODULE_RESOURCE_H__