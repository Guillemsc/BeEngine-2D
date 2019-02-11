#include "ResourceScene.h"
#include "App.h"
#include "ModuleResource.h"
#include "imgui.h"

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
	std::string library_path = App->resource->GetLibraryPathFromResourceType(GetType());

	std::string filepath = library_path + uid + ".scene";

	std::string resultant_filepath;
	if (App->file_system->FileCopyPaste(GetAssetFilepath(), library_path, true, resultant_filepath))
	{
		std::string new_filepath = library_path + uid + ".scene";
		if (App->file_system->FileExists(new_filepath))
			App->file_system->FileDelete(new_filepath);

		App->file_system->FileRename(resultant_filepath, uid);
	}
}

void ResourceScene::ImportFromLibrary()
{
	std::string resource_filepath = GetLibraryFilepath();

	if (App->file_system->FileExists(resource_filepath))
	{
		abstraction.DeSerialize(resource_filepath);

		JSON_Doc* doc = App->json->LoadJSON(resource_filepath.c_str());

		related_scenes.clear();

		if (doc != nullptr)
		{
			uint count = doc->GetArrayCount("sub_scenes");

			for (int i = 0; i < count; ++i)
			{
				std::string uid = doc->GetStringFromArray("sub_scenes", i);

				related_scenes.push_back(uid);
			}
		}

		App->json->UnloadJSON(doc);
	}
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

void ResourceScene::LoadToScene()
{

}

bool ResourceScene::DrawEditorExplorer()
{
	bool ret = false;

	if (ImGui::Button("Load Scene"))
	{
		LoadToScene();

		ret = true;
	}

	return ret;
}
