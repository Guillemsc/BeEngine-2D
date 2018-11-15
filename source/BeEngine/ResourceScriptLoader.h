#ifndef __RESOURCE_SCRIPT_LOADER_H__
#define __RESOURCE_SCRIPT_LOADER_H__

#include "ResourceLoader.h"
#include "ModuleFileSystem.h"

class ResourceScriptLoader : public ResourceLoader
{
public:
	ResourceScriptLoader();

	void Start();
	void CleanUp();

	Resource* CreateResource(std::string new_uid);

	void CreateLibraryFolder();

	bool ExportAssetToLibrary(DecomposedFilePath decomposed_file_path, std::string new_uid);
	bool ClearAssetDataFromEngine(DecomposedFilePath decomposed_file_path);
	bool DeleteAssetResources(DecomposedFilePath decomposed_file_path);
	bool RenameAsset(DecomposedFilePath decomposed_file_path, const char* new_name);
	bool IsAssetOnLibrary(DecomposedFilePath decomposed_file_path, std::vector<std::string>& library_files_used);
	bool ImportAssetFromLibrary(DecomposedFilePath decomposed_file_path, std::vector<Resource*>& resources = std::vector<Resource*>());
	bool ExportResourceToLibrary(Resource* resource);

	bool CreateScriptAsset(const char* script_name, const char* path);

private:
	void OnDestroyResource(Resource* res);
};

#endif // !__RESOURCE_SCRIPT_LOADER_H__