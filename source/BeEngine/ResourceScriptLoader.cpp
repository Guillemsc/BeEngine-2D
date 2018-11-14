#include "ResourceScriptLoader.h"
#include "App.h"
#include "ModuleResource.h"
#include "ResourceScript.h"
#include "ModuleJson.h"
#include "ModuleScripting.h"
#include "ScriptingObjectCompiler.h"

ResourceScriptLoader::ResourceScriptLoader() : ResourceLoader(ResourceType::SCRIPT, "Scripts")
{
}

void ResourceScriptLoader::Start()
{
}

void ResourceScriptLoader::CleanUp()
{
}

Resource * ResourceScriptLoader::CreateResource(std::string new_uid)
{
	return new ResourceScript(new_uid);
}

void ResourceScriptLoader::CreateLibraryFolder()
{
	library_path = App->file_system->CreateFolder(App->resource->GetLibraryPath().c_str(), "scripts");
}

bool ResourceScriptLoader::ExportAssetToLibrary(DecomposedFilePath d_filepath, std::string new_uid)
{
	std::string dll_output_path = library_path + new_uid + ".dll";
	App->scripting->compiler->CompileScript(d_filepath.file_path.c_str(), dll_output_path.c_str());

	App->file_system->FileCopyPasteWithNewName(d_filepath.file_path.c_str(), library_path.c_str(), new_uid.c_str());

	// Crate assets meta
	std::string assets_meta_path = d_filepath.file_path + ".meta";

	if (App->file_system->FileExists(assets_meta_path.c_str()))
		App->file_system->FileDelete(assets_meta_path.c_str());

	JSON_Doc* doc = App->json->CreateJSON(assets_meta_path.c_str());
	if (doc != nullptr)
	{
		doc->SetString("resource", new_uid.c_str());

		doc->Save();
		App->json->UnloadJSON(doc);
	}

	return false;
}

bool ResourceScriptLoader::ClearAssetDataFromEngine(DecomposedFilePath decomposed_file_path)
{
	return false;
}

bool ResourceScriptLoader::DeleteAssetResources(DecomposedFilePath decomposed_file_path)
{
	return false;
}

bool ResourceScriptLoader::RenameAsset(DecomposedFilePath decomposed_file_path, const char * new_name)
{
	return false;
}

bool ResourceScriptLoader::IsAssetOnLibrary(DecomposedFilePath decomposed_file_path, std::vector<std::string>& library_files_used)
{
	return false;
}

bool ResourceScriptLoader::ImportAssetFromLibrary(DecomposedFilePath decomposed_file_path, std::vector<Resource*>& resources)
{
	return false;
}

bool ResourceScriptLoader::ExportResourceToLibrary(Resource * resource)
{
	return false;
}

void ResourceScriptLoader::OnDestroyResource(Resource * res)
{
}
