#ifndef __RESOURCE_FONT_H__
#define __RESOURCE_FONT_H__

#include "Resource.h"
#include "Globals.h"
#include "ModuleScripting.h"
#include "GameObjectAbstraction.h"
#include "ModuleText.h"

class ResourceScript;
class GameObject;
class GameObjectComponent;

class ResourceFont : public Resource
{
	friend class ModuleResource;
	friend class ModuleAssets;

private:
	void operator delete(void *) {}

public:
	ResourceFont();

	void CleanUp();

	bool ExistsOnLibrary(std::string uid, std::string& library_filepath = std::string());
	void ExportToLibrary(std::string uid);
	void ImportFromLibrary();

	void OnRemoveAsset();
	void OnRenameAsset(const char* new_name, const char* last_name);
	void OnMoveAsset(const char* new_asset_path, const char* last_asset_path);

	bool DrawEditorExplorer();

private:
	FT_Face face;

};

#endif // !__RESOURCE_FONT_H__