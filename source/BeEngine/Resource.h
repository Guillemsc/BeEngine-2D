#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include <string>
#include <vector>

#include "ModuleFileSystem.h"

enum ResourceType
{
	RESOURCE_TYPE_UNKWNOWN,
	RESOURCE_TYPE_TEXTURE,
	RESOURCE_TYPE_SCRIPT,
	RESOURCE_TYPE_SHADER,
};

class Resource
{
	friend class ResourceLoader;
	friend class ModuleResource;

private:
	void operator delete(void *) {}

public:
	Resource(ResourceType resource_type);

	// Assets and library management (Editor Mode)
	void EM_InitResource(const char* asset_filepath);
	void EM_CreateAssetMeta();
	bool EM_ExistsOnLibrary();
	void EM_LoadLibraryFilepaths();
	void EM_ExportToLibrary();
	void EM_ImportFromLibrary();
	void EM_RemoveAsset();
	void EM_RenameAsset(const char* new_name);
	void EM_MoveAsset(const char* new_path);

	// Library standalone management (Game Mode)
	void GM_InitResource(const char* library_filepath);
	void GM_ImportFromLibrary();

	std::string GetMetaFilepath() const;
	std::string GetAssetFilepath() const;
	std::string GetLibraryFilepath() const;
	DecomposedFilePath GetDecomposedAssetFilepath() const;
	const ResourceType GetType() const;
	std::string GetUID() const;

	virtual void CleanUp();

protected:
	void SetAssetFilepath(const std::string& set);

private:
	bool AssetFileExists() const;
	bool MetaFileExists() const;

private:
	virtual bool ExistsOnLibrary(std::string uid, std::string& library_filepath = std::string()) = 0;
	virtual void ExportToLibrary(std::string uid) = 0;
	virtual void ImportFromLibrary() = 0;

	virtual void OnRemoveAsset() = 0;
	virtual void OnRenameAsset(const char* new_name, const char* last_name) = 0;
	virtual void OnMoveAsset(const char* new_path, const char* last_path) = 0;

private:
	bool has_data = false;

	ResourceType type = ResourceType::RESOURCE_TYPE_UNKWNOWN;

	std::string uid;
	std::string meta_filepath;
	std::string asset_filepath;
	std::string library_filepath;

	DecomposedFilePath d_asset_filepath;
};

#endif // !__RESOURCE_H__