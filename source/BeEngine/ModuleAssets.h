#ifndef __MODULE_ASSETS_H__
#define __MODULE_ASSETS_H__

#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"

#include <map>

#include "Module.h"
#include "Timer.h"
#include "Resource.h"
#include "ModuleTimeSlicedTask.h"

class Event;
class LoadResourcesTimeSlicedTask;
class ManageModifiedAssetsTimeSlicedTask;
class ResourceScript;

class ModuleAssets : public Module
{
public:
	ModuleAssets();
	~ModuleAssets();

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

	void ForceUpdateFolders();

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
	void CreateLibraryFolders();

	void AddToFoldersToCheck(std::string folder);
	void ManageFoldersToCheck();

	void StartLoadResourcesTimeSlicedTask();
	void StartManageModifiedAssetsTimeSlicedTask(const std::string& folder);

private:
	std::string current_assets_folder;
	std::string assets_folder;
	std::string library_folder;

	std::vector<std::string> folders_to_update;
	Timer folders_to_update_timer;
	bool force_update_folders = false;

	int rising_watching_events_index = 0;

	LoadResourcesTimeSlicedTask* time_sliced_task_loading_resources = nullptr;
	ManageModifiedAssetsTimeSlicedTask* time_sliced_task_manage_modified_assets = nullptr;
};

class ManageModifiedAssetsTimeSlicedTask : public TimeSlicedTask
{
public:
	enum ManageModifiedAssetsState
	{
		CHECK_EXISTING_ASSETS,
		CHECK_FOR_DELETED_ASSETS,
	};

	ManageModifiedAssetsTimeSlicedTask(std::string folder_to_check);

	void Start();
	void Update();
	void Finish();

private:
	void CheckExisitingAssets();
	void CheckForDeletedAssets();

private:
	ManageModifiedAssetsState state;

	std::string folder_to_check;

	std::vector<std::string> all_asset_files;
	std::vector<std::string> asset_files_to_check;

	std::vector<Resource*> all_asset_resources;
	std::vector<Resource*> asset_resources_to_check;
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

#endif // !__MODULE_ASSETS_H__