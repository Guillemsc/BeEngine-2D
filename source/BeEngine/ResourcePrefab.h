#ifndef __RESOURCE_PREFAB_H__
#define __RESOURCE_PREFAB_H__

#include "Resource.h"
#include "Globals.h"
#include "ModuleScripting.h"
#include "GameObjectAbstraction.h"

class ResourceScript;
class GameObject;
class GameObjectComponent;

class ResourcePrefab : public Resource
{
	friend class ModuleResource;
	friend class ModuleAssets;

private:
	void operator delete(void *) {}

public:
	ResourcePrefab();

	void CleanUp();

	bool ExistsOnLibrary(std::string uid, std::string& library_filepath = std::string());
	void ExportToLibrary(std::string uid);
	void ImportFromLibrary();

	void OnRemoveAsset();
	void OnRenameAsset(const char* new_name, const char* last_name);
	void OnMoveAsset(const char* new_asset_path, const char* last_asset_path);

	void UpdatePrefab(const GameObjectAbstraction& abstraction);

	bool DrawEditorExplorer();

private:
	GameObjectAbstraction abstraction;
};

#endif // !__RESOURCE_PREFAB_H__