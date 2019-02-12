#include "ResourceScene.h"
#include "App.h"
#include "ModuleResource.h"
#include "imgui.h"
#include "ModuleGameObject.h"
#include "Scene.h"
#include "ModuleAssets.h"

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

		if (doc != nullptr)
		{
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

void ResourceScene::UpdateScene(Scene* sc, std::vector<std::string> used_uids)
{
	App->assets->StopRisingWatchingEvents();

	if (sc != nullptr)
	{
		std::vector<GameObject*> root_gos = sc->GetRootGameObjects();

		abstraction.Abstract(root_gos);

		related_scenes.clear();

		abstraction.Serialize(d_asset_filepath.path, d_asset_filepath.file_name, "scene");

		JSON_Doc* doc = App->json->LoadJSON(d_asset_filepath.file_path.c_str());

		if (doc != nullptr)
		{
			doc->SetArray("sub_scenes");

			for (std::vector<std::string>::iterator it = used_uids.begin(); it != used_uids.end(); ++it)
			{
				doc->AddStringToArray("sub_scenes", (*it).c_str());
				related_scenes.push_back((*it));
			}

			doc->Save();

			App->json->UnloadJSON(doc);
		}

		ExportToLibrary(uid);

		App->assets->RenameAsset(asset_filepath.c_str(), sc->GetName().c_str());
	}

	App->assets->StartRisingWatchingEvents();
}

std::vector<GameObject*> ResourceScene::LoadToScene(Scene* sc)
{
	std::vector<GameObject*> ret;

	if (sc != nullptr)
	{
		ret = abstraction.DeAbstract();

		sc->SetResourceScene(this);
		sc->SetName(GetDecomposedAssetFilepath().file_name.c_str());

		for (std::vector<GameObject*>::iterator it = ret.begin(); it != ret.end(); ++it)
		{
			App->gameobject->SetGameObjectScene(sc, (*it));
		}

		for (std::vector<std::string>::iterator it = related_scenes.begin(); it != related_scenes.end(); ++it)
		{
			ResourceScene* sc = (ResourceScene*)App->resource->GetResourceFromUid((*it), ResourceType::RESOURCE_TYPE_SCENE);

			if (sc != nullptr)
			{
				Scene* new_scene = App->gameobject->CreateSubScene();
				sc->LoadToScene(new_scene);
			}
		}
	}

	return ret;
}

bool ResourceScene::DrawEditorExplorer()
{
	bool ret = false;

	if (ImGui::Button("Load Scene"))
	{
		App->gameobject->DestroyScene(App->gameobject->GetRootScene());
		LoadToScene(App->gameobject->GetRootScene());

		ret = true;
	}

	return ret;
}
