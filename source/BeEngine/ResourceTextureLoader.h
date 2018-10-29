#ifndef __RESOURCE_TEXTURE_LOADER_H__
#define __RESOURCE_TEXTURE_LOADER_H__

#include "ResourceLoader.h"
#include "ModuleFileSystem.h"

class ResourceTextureLoader : public ResourceLoader
{
public:
	ResourceTextureLoader();

	void Start();
	void CleanUp();

	Resource* CreateResource(std::string new_uid);

	void CreateLibraryFolder();

	bool ImportAssetToEngine(DecomposedFilePath decomposed_file_path, std::vector<Resource*>& resources);
	bool ClearAssetDataFromEngine(DecomposedFilePath decomposed_file_path);
	bool DeleteAssetResources(DecomposedFilePath decomposed_file_path);
	bool RenameAsset(DecomposedFilePath decomposed_file_path, const char* new_name);
	bool IsAssetOnLibrary(DecomposedFilePath decomposed_file_path, std::vector<std::string>& library_files_used);
	bool ImportResourceFromLibrary(DecomposedFilePath decomposed_file_path);
	bool ExportResourceToLibrary(Resource* resource);

private:
	void OnDestroyResource(Resource* res);
};

#endif // !__RESOURCE_TEXTURE_LOADER_H__