#include "ResourceFont.h"
#include "App.h"
#include "ModuleResource.h"
#include "ScriptingBridgeResourceFont.h"

ResourceFont::ResourceFont() 
	: Resource(new ScriptingBridgeResourceFont(this), ResourceType::RESOURCE_TYPE_FONT)
{
}

void ResourceFont::CleanUp()
{
	App->text->UnloadFace(face);
}

bool ResourceFont::ExistsOnLibrary(std::string uid, std::string & library_filepath)
{
	bool ret = false;

	std::string library_path = App->resource->GetLibraryPathFromResourceType(GetType());

	std::string path = library_path + uid + "." + d_asset_filepath.file_extension;

	if (App->file_system->FileExists(path.c_str()))
	{
		library_filepath = path;
		ret = true;
	}

	return ret;
}

void ResourceFont::ExportToLibrary(std::string uid)
{
	std::string library_path = App->resource->GetLibraryPathFromResourceType(GetType());

	std::string resultant_filepath;
	if (App->file_system->FileCopyPaste(GetAssetFilepath(), library_path, true, resultant_filepath))
	{
		std::string new_filepath = library_path + uid + "." + d_asset_filepath.file_extension;

		if (App->file_system->FileExists(new_filepath))
			App->file_system->FileDelete(new_filepath);

		App->file_system->FileRename(resultant_filepath, uid);
	}
}

void ResourceFont::ImportFromLibrary()
{
	std::string resource_filepath = GetLibraryFilepath();

	if (App->file_system->FileExists(resource_filepath))
	{
		App->text->LoadFace(resource_filepath.c_str(), face);
	}
}

void ResourceFont::OnRemoveAsset()
{
}

void ResourceFont::OnRenameAsset(const char * new_name, const char * last_name)
{
}

void ResourceFont::OnMoveAsset(const char * new_asset_path, const char * last_asset_path)
{
}

bool ResourceFont::DrawEditorExplorer()
{
	return false;
}

FT_Face ResourceFont::GetFace() const
{
	return face;
}
