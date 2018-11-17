#ifndef __MODULE_RESOURCE_H__
#define __MODULE_RESOURCE_H__

#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"

#include <map>

#include "Module.h"
#include "Resource.h"
#include "ModuleTimeSlicedTask.h"

class Event;

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

	void OnEvent(Event* ev);

	std::string GetNewUID();

	void SetCurrentAssetsPath(const char* path);
	std::string GetCurrentAssetsPath();
	std::string GetLibraryPath();
	std::string GetAssetsPath();

	std::string GetLibraryPathFromResourceType(const ResourceType& type);

	Resource* CreateResource(const ResourceType type);
	void DestroyResource(Resource* res);
	Resource* GetResourceFromAssetFile(const char* filepath);
	std::vector<Resource*> GetAllResources() const;

	bool LoadFileToEngine(const char* filepath);
	void UnloadAssetFromEngine(const char* filepath);
	bool ExportAssetToLibrary(const char* filepath);
	bool ImportAsset(const char* filepath, Resource*& res);
	bool ManageModifiedAsset(const char* filepath);
	bool RenameAsset(const char* filepath, const char* new_name);

	bool CanLoadFile(const char* filepath);
	bool IsMeta(const char* filepath);
	const char* GetAssetFileFromMeta(const char* metapath);
	const char* GetMetaFileFromAsset(const char* filepath);

	void StartWatchingFolders();
	void StopWatchingFolders();

private:
	void AddAssetExtension(const ResourceType& type, const char* extension);
	void AddLibraryExtension(const ResourceType& type, const char* extension);

	ResourceType GetResourceTypeFromAssetExtension(const char* extension);
	ResourceType GetResourceTypeFromLibraryExtension(const char* extension);

	void AddLibraryName(const ResourceType& type, const char* name);

	void CreateLibraryFolders();

	void ManageFilesToCheck();

	void DestroyAllResources();

private:
	std::string current_assets_folder;
	std::string assets_folder;
	std::string library_folder;

	std::map<ResourceType, std::vector<std::string>> asset_extensions;
	std::map<ResourceType, std::vector<std::string>> library_extensions;
	std::map<ResourceType, std::string> library_names;

	std::vector<Resource*> resources;

	std::vector<std::string> files_changed_to_check;

	int watching_folder_index = 0;
};

class LoadResourcesTimeSlicedTask : public TimeSlicedTask
{
public:
	enum LoadResourcesState
	{
		CHECK_ASSET_FILES_IMPORT,
		CLEAN_ASSET_FOLDER,
		CLEAN_LIBRARY_FOLDER,
		DELETE_UNNECESSARY_FILES,
	};

	LoadResourcesTimeSlicedTask();

	void Start();
	void Update();
	void Finish();

private:
	void CheckAssetFilesImport();
	void CleanAssetFolder();
	void CleanLibraryFolder();
	void DeleteUnnecessaryFiles();

private:
	LoadResourcesState state;

	std::vector<std::string> all_asset_files;
	std::vector<std::string> all_library_files;

	std::vector<std::string> asset_files_to_check;
	uint all_asset_files_to_check_count = 0;

	std::vector<std::string> library_files_to_check;
	uint all_library_files_to_check_count = 0;

	std::vector<std::string> files_to_delete;

	std::vector<std::string> asset_files_used;
	std::vector<std::string> library_files_used;
};

#endif // !__MODULE_RESOURCE_H__