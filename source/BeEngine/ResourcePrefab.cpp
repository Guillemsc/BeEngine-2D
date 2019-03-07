#include "ResourcePrefab.h"
#include "App.h"
#include "ModuleResource.h"
#include "ModuleAssets.h"
#include "imgui.h"
#include "GameObject.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

ResourcePrefab::ResourcePrefab() : Resource(ResourceType::RESOURCE_TYPE_PREFAB)
{
}

void ResourcePrefab::CleanUp()
{
}

bool ResourcePrefab::ExistsOnLibrary(std::string uid, std::string & library_filepath)
{
	bool ret = false;

	std::string library_path = App->resource->GetLibraryPathFromResourceType(GetType());

	std::string path = library_path + uid + ".prefab";

	if (App->file_system->FileExists(path.c_str()))
	{
		library_filepath = path;
		ret = true;
	}

	return ret;
}

void ResourcePrefab::ExportToLibrary(std::string uid)
{
	std::string library_path = App->resource->GetLibraryPathFromResourceType(GetType());

	std::string filepath = library_path + uid + ".prefab";

	std::string resultant_filepath;
	if (App->file_system->FileCopyPaste(GetAssetFilepath(), library_path, true, resultant_filepath))
	{
		std::string new_filepath = library_path + uid + ".prefab";
		if (App->file_system->FileExists(new_filepath))
			App->file_system->FileDelete(new_filepath);

		App->file_system->FileRename(resultant_filepath, uid);
	}
}

void ResourcePrefab::ImportFromLibrary()
{
	std::string resource_filepath = GetLibraryFilepath();

	if (App->file_system->FileExists(resource_filepath))
	{
		abstraction.DeSerialize(resource_filepath);
	}
}

void ResourcePrefab::OnRemoveAsset()
{
}

void ResourcePrefab::OnRenameAsset(const char * new_name, const char * last_name)
{
}

void ResourcePrefab::OnMoveAsset(const char * new_asset_path, const char * last_asset_path)
{
}

void ResourcePrefab::UpdatePrefab(const GameObjectAbstraction & abs)
{
	abstraction = abs;

	App->assets->StopRisingWatchingEvents();

	abstraction.Serialize(d_asset_filepath.path, d_asset_filepath.file_name, "prefab");

	ExportToLibrary(uid);

	App->assets->StartRisingWatchingEvents();
}

void ResourcePrefab::LoadToScene()
{
	std::vector<GameObject*> gos = abstraction.DeAbstract();

	for (std::vector<GameObject*>::iterator it = gos.begin(); it != gos.end(); ++it)
	{
		(*it)->resource_prefab = this;
	}
}

bool ResourcePrefab::DrawEditorExplorer()
{
	bool ret = false;

	if (ImGui::Button("Load Prefab"))
	{
		LoadToScene();

		ret = true;
	}

	return ret;
}
