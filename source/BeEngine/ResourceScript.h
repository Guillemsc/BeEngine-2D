#ifndef __RESOURCE_SCRIPT_H__
#define __RESOURCE_SCRIPT_H__

#include "Resource.h"
#include "Globals.h"
#include <vector>

class ResourceScript : public Resource
{
	friend class ResourceScriptLoader;
	friend class ModuleResource;

private:
	void operator delete(void *) {}

public:
	ResourceScript();

	void CleanUp();

	bool ExistsOnLibrary(std::string uid, std::string& library_filepath = std::string());
	void ExportToLibrary(std::string uid);
	void ImportFromLibrary();

	void OnRemoveAsset();
	void OnRenameAsset(const char* new_name, const char* last_name);

	void Compile();

private:
	void SaveCodeOnMeta();
	bool GetCodeDifferentFromMeta();

private:
	bool compiles = false;

	std::vector<std::string> compile_errors;
};

#endif // !__RESOURCE_SCRIPT_H__