#ifndef __RESOURCE_SCENE_H__
#define __RESOURCE_SCENE_H__

#include "Resource.h"
#include "Globals.h"
#include "ModuleScripting.h"
#include "GameObjectAbstraction.h"

class ResourceScript;
class GameObject;
class GameObjectComponent;
class Scene;

class ResourceScene : public Resource
{
	friend class ModuleResource;
	friend class ModuleAssets;

private:
	void operator delete(void *) {}

public:
	ResourceScene();

	void CleanUp();

	bool ExistsOnLibrary(std::string uid, std::string& library_filepath = std::string());
	void ExportToLibrary(std::string uid);
	void ImportFromLibrary();

	void OnRemoveAsset();
	void OnRenameAsset(const char* new_name, const char* last_name);
	void OnMoveAsset(const char* new_asset_path, const char* last_asset_path);

	void UpdateScene(Scene* sc, std::vector<std::string> used_uids = std::vector<std::string>());
	std::vector<GameObject*> LoadToScene(Scene* sc);

	bool DrawEditorExplorer();

private:
	GameObjectAbstraction abstraction;

	std::vector<std::string> related_scenes;
};

#endif // !__RESOURCE_PREFAB_H__