#include "Resource.h"
#include "App.h"
#include "ModuleFileSystem.h"
#include "ModuleResource.h"
#include "ModuleJson.h"

Resource::Resource(ResourceType resource_type)
{
	type = resource_type;
}

void Resource::EM_InitResource(const char * _asset_filepath)
{
	if (App->file_system->FileExists(_asset_filepath))
	{
		asset_filepath = _asset_filepath;
		uid = "";

		EM_CreateAssetMeta();

		has_data = true;

		if (EM_ExistsOnLibrary())
		{
			EM_LoadLibraryFilepaths();

			EM_ImportFromLibrary();
		}
		else
		{
			EM_ExportToLibrary();

			EM_ImportFromLibrary();
		}
	}
}

void Resource::EM_CreateAssetMeta()
{
	meta_filepath = asset_filepath + ".meta";

	JSON_Doc* meta_doc = App->json->LoadJSON(meta_filepath.c_str());

	if (meta_doc == nullptr)
	{
		meta_doc = App->json->CreateJSON(meta_filepath.c_str());
	}
	else
	{
		uid = meta_doc->GetString("resource", "");
	}

	if (uid == "")
	{
		uid = App->resource->GetNewUID();

		meta_doc->SetString("resource", uid.c_str());
	}
	else
	{
		meta_doc->SetString("resource", uid.c_str());
	}

	meta_doc->Save();

	App->json->UnloadJSON(meta_doc);
}

bool Resource::EM_ExistsOnLibrary()
{
	bool ret = false;

	if (has_data)
	{
		ret = ExistsOnLibrary(uid);
	}

	return ret;
}

void Resource::EM_LoadLibraryFilepaths()
{
	if (has_data)
	{
		ExistsOnLibrary(uid, library_filepath);
	}
}

void Resource::EM_ExportToLibrary()
{
	if (has_data)
	{
		ExportToLibrary(uid);

		EM_LoadLibraryFilepaths();
	}
}

void Resource::EM_ImportFromLibrary()
{
	if (has_data)
	{
		if (EM_ExistsOnLibrary())
		{
			ImportFromLibrary();
		}
	}
}

void Resource::EM_RemoveAsset()
{
	if (has_data)
	{
		if(App->file_system->FileExists(asset_filepath.c_str()))
			App->file_system->FileDelete(asset_filepath.c_str());

		if (App->file_system->FileExists(meta_filepath.c_str()))
			App->file_system->FileDelete(meta_filepath.c_str());

		if (App->file_system->FileExists(library_filepath.c_str()))
			App->file_system->FileDelete(library_filepath.c_str());

		OnRemoveAsset();
	}
}

void Resource::EM_RenameAsset(const char * new_name)
{
	DecomposedFilePath df = App->file_system->DecomposeFilePath(asset_filepath.c_str());

	std::string last_name = df.file_name;

	if (App->file_system->FileRename(asset_filepath.c_str(), new_name, true, asset_filepath))
	{
		DecomposedFilePath new_df = App->file_system->DecomposeFilePath(asset_filepath.c_str());

		std::string new_meta_name = new_df.file_name + "." + new_df.file_extension;

		if (App->file_system->FileRename(meta_filepath.c_str(), new_meta_name.c_str(), false, meta_filepath))
		{
			OnRenameAsset(new_df.file_name.c_str(), last_name.c_str());
		}
	}
}

void Resource::GM_InitResource(const char * _library_filepath)
{
	if (App->file_system->FileExists(_library_filepath))
	{
		library_filepath = _library_filepath;

		DecomposedFilePath d_filepath = App->file_system->DecomposeFilePath(_library_filepath);

		uid = d_filepath.file_name;

		has_data = true;

		GM_ImportFromLibrary();
	}
}

void Resource::GM_ImportFromLibrary()
{
	if (has_data)
	{
		ImportFromLibrary();
	}
}

std::string Resource::GetMetaFilepath() const
{
	return meta_filepath;
}

std::string Resource::GetAssetFilepath() const
{
	return asset_filepath;
}

std::string Resource::GetLibraryFilepath() const
{
	return library_filepath;
}

const ResourceType Resource::GetType() const
{
	return type;
}

std::string Resource::GetUID() const
{
	return uid;
}

void Resource::CleanUp()
{
}

bool Resource::AssetFileExists() const
{
	bool ret = false;

	if (has_data)
	{
		ret = App->file_system->FileExists(asset_filepath.c_str());
	}

	return ret;
}

bool Resource::MetaFileExists() const
{
	bool ret = false;

	if (has_data)
	{
		std::string meta_path = asset_filepath + ".meta";

		ret = App->file_system->FileExists(meta_path.c_str());
	}

	return ret;
}
