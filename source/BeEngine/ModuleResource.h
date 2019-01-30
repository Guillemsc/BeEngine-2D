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

	void OnEvent(Event* ev);

	std::string GetNewUID();

	void SetCurrentAssetsPath(const char* path);
	std::string GetCurrentAssetsPath();
	std::string GetLibraryPath();
	std::string GetAssetsPath();

	std::string GetResourceNameFromResourceType(const ResourceType& type);
	std::string GetLibraryPathFromResourceType(const ResourceType& type);

	Resource* CreateResource(const ResourceType type);
	void DestroyResource(Resource* res);
	Resource* GetResourceFromAssetFile(const char* filepath);
	std::vector<Resource*> GetResourcesFromResourceType(const ResourceType type);
	std::map<ResourceType, std::vector<Resource*>> GetAllResources() const;

	bool EditorResourceSelector(const ResourceType type, Resource *& res, std::string& filter_text);

	// Asset management ---------

	bool LoadFileToEngine(const char* filepath);
	bool ManageModifiedAsset(const char* filepath);

	void UnloadAssetFromEngine(const char* filepath);
	bool ExportAssetToLibrary(const char* filepath);
	bool ImportAsset(const char* filepath, Resource*& res);
	bool RenameAsset(const char* filepath, const char* new_name);
	bool MoveAsset(const char* filepath, const char* new_path);

	bool CreateScript(const char* filepath, const char* name);

	bool MoveAssetsFolder(const char* folder, const char* new_path);
	bool CreateAssetsFolder(const char* path, const char* name);
	bool RenameAssetsFolder(const char* folder_path, const char* new_name);
	bool DeleteAssetsFolder(const char* folder);

	// --------------------------

	void LoadUserScriptsInfo();

	bool CanLoadFile(const char* filepath);
	bool IsMeta(const char* filepath);
	std::string GetAssetFileFromMeta(const char* metapath);
	std::string GetMetaFileFromAsset(const char* filepath);

	void StartWatchingFolders();
	void StopWatchingFolders();
	void StopRisingWatchingEvents();
	void StartRisingWatchingEvents();

private:
	void AddAssetExtension(const ResourceType& type, const char* extension);
	void AddLibraryExtension(const ResourceType& type, const char* extension);

	ResourceType GetResourceTypeFromAssetExtension(const char* extension);
	ResourceType GetResourceTypeFromLibraryExtension(const char* extension);

	void AddResourceName(const ResourceType& type, const char* name);

	void CreateLibraryFolders();

	void ManageFilesToCheck();

	void DestroyAllResources();

	void StartLoadResourcesTimeSlicedTask();
	void StartManageModifiedAssetsTimeSlicedTask(const std::string& folder, bool check_sub_directories = false);

private:
	std::string current_assets_folder;
	std::string assets_folder;
	std::string library_folder;

	std::map<ResourceType, std::vector<std::string>> asset_extensions;
	std::map<ResourceType, std::vector<std::string>> library_extensions;
	std::map<ResourceType, std::string> resource_names;

	std::map<ResourceType, std::vector<Resource*>> resources;

	std::vector<std::string> files_changed_to_check;

	int rising_watching_events_index = 0;

	LoadResourcesTimeSlicedTask* time_sliced_task_loading_resources = nullptr;
	ManageModifiedAssetsTimeSlicedTask* time_sliced_task_manage_modified_assets = nullptr;
};

class ManageModifiedAssetsTimeSlicedTask : public TimeSlicedTask
{
public:
	ManageModifiedAssetsTimeSlicedTask(std::string folder_to_check, bool check_sub_directories);

	void Start();
	void Update();
	void Finish();

private:
	std::string folder_to_check;
	bool check_sub_directories = false;

	std::vector<std::string> all_asset_files;
	std::vector<std::string> asset_files_to_check;
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
	uint all_files_to_delete_count = 0;

	std::vector<std::string> asset_files_used;
	std::vector<std::string> library_files_used;
};

#endif // !__MODULE_RESOURCE_H__