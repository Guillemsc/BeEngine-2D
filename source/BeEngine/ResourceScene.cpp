#include "ResourceScene.h"
#include "App.h"
#include "ModuleResource.h"

ResourceScene::ResourceScene() : Resource(ResourceType::RESOURCE_TYPE_SCENE)
{
}

void ResourceScene::CleanUp()
{
}

bool ResourceScene::ExistsOnLibrary(std::string uid, std::string & library_filepath)
{
	bool ret = false;

	std::string library_path = App->resource->GetLibraryPathFromResourceType(GetType());

	std::string path = library_path + uid + ".scene";

	if (App->file_system->FileExists(path.c_str()))
	{
		library_filepath = path;
		ret = true;
	}

	return ret;
}

void ResourceScene::ExportToLibrary(std::string uid)
{
}

void ResourceScene::ImportFromLibrary()
{
}

void ResourceScene::OnRemoveAsset()
{
}

void ResourceScene::OnRenameAsset(const char * new_name, const char * last_name)
{
}

void ResourceScene::OnMoveAsset(const char * new_asset_path, const char * last_asset_path)
{
}

void ResourceScene::UpdateScene(const GameObjectAbstraction & abstraction)
{
}

bool ResourceScene::DrawEditorExplorer()
{
	return false;
}
