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
		ret = OnExistsOnLibrary(uid);
	}

	return ret;
}

void Resource::EM_LoadLibraryFilepaths()
{
	if (has_data)
	{
		OnExistsOnLibrary(uid, library_filepath);
	}
}

void Resource::EM_ExportToLibrary()
{
	if (has_data)
	{
		OnExportToLibrary(uid);

		EM_LoadLibraryFilepaths();
	}
}

void Resource::EM_ImportFromLibrary()
{
	if (has_data)
	{
		if (EM_ExistsOnLibrary())
		{
			OnImportFromLibrary();
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
		OnImportFromLibrary();
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
