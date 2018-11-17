#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include <string>
#include <vector>

enum ResourceType
{
	UNKWNOWN,
	TEXTURE,
	SCRIPT,
	SHADER,
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

	// Library standalone management (Game Mode)
	void GM_InitResource(const char* library_filepath);
	void GM_ImportFromLibrary();

	std::string GetMetaFilepath() const;
	std::string GetAssetFilepath() const;
	std::string GetLibraryFilepath() const;
	const ResourceType GetType() const;
	std::string GetUID() const;

	virtual void CleanUp();

private:
	bool AssetFileExists() const;
	bool MetaFileExists() const;

private:
	virtual bool OnExistsOnLibrary(std::string uid, std::string& library_filepath = std::string()) = 0;
	virtual void OnExportToLibrary(std::string uid) = 0;
	virtual void OnImportFromLibrary() = 0;

private:
	bool has_data = false;

	ResourceType type = ResourceType::UNKWNOWN;

	std::string uid;
	std::string meta_filepath;
	std::string asset_filepath;
	std::string library_filepath;
};

#endif // !__RESOURCE_H__