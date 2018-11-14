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
	bool ret = false;

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

		ret = true;
	}

	return ret;
}

bool ResourceScriptLoader::ClearAssetDataFromEngine(DecomposedFilePath d_filepath)
{
	bool ret = false;

	std::string assets_meta_path = d_filepath.file_path + ".meta";

	JSON_Doc* doc = App->json->LoadJSON(assets_meta_path.c_str());

	if (doc != nullptr)
	{
		std::string uid = doc->GetString("resource");

		std::string resource_1 = uid += ".dll";
		std::string resource_2 = uid += ".cs";

		std::string resource_1_filepath = library_path + resource_1;
		std::string resource_2_filepath = library_path + resource_2;

		if (App->file_system->FileExists(resource_1_filepath.c_str()))
			App->file_system->FileDelete(resource_1_filepath.c_str());

		if (App->file_system->FileExists(resource_2_filepath.c_str()))
			App->file_system->FileDelete(resource_2_filepath.c_str());

		App->json->UnloadJSON(doc);
		App->file_system->FileDelete(assets_meta_path.c_str());

		ret = true;
	}

	return ret;
}

bool ResourceScriptLoader::DeleteAssetResources(DecomposedFilePath d_filepath)
{
	bool ret = false;

	std::string assets_meta_path = d_filepath.file_path + ".meta";

	JSON_Doc* doc = App->json->LoadJSON(assets_meta_path.c_str());

	if (doc != nullptr)
	{
		std::string resource = doc->GetString("resource");

		ret = App->resource->DestroyResource(resource, ResourceType::SCRIPT);

		App->json->UnloadJSON(doc);

		ret = true;
	}

	return ret;
}

bool ResourceScriptLoader::RenameAsset(DecomposedFilePath d_filepath, const char * new_name)
{
	bool ret = false;

	App->file_system->FileRename(d_filepath.file_path.c_str(), new_name);

	std::string assets_meta_path = d_filepath.file_path + ".meta";

	if (App->file_system->FileExists(assets_meta_path.c_str()))
	{
		std::string meta_new_name = new_name + std::string(".") + d_filepath.file_extension;

		App->file_system->FileRename(assets_meta_path.c_str(), meta_new_name.c_str());

		ret = true;
	}

	return ret;
}

bool ResourceScriptLoader::IsAssetOnLibrary(DecomposedFilePath d_filepath, std::vector<std::string>& library_files_used)
{
	bool ret = false;

	std::string assets_meta_path = d_filepath.file_path + ".meta";

	JSON_Doc* doc = App->json->LoadJSON(assets_meta_path.c_str());

	if (doc != nullptr)
	{
		std::string uid = doc->GetString("resource");

		std::string resource_1 = uid += ".dll";
		std::string resource_2 = uid += ".cs";

		std::string resource_1_filepath = library_path + resource_1;
		std::string resource_2_filepath = library_path + resource_2;

		if (App->file_system->FileExists(resource_2_filepath.c_str()))
		{
			library_files_used.push_back(resource_2_filepath);

			ret = true;

			// It is possible that there is no dll because of script not compiling
			if (App->file_system->FileExists(resource_1_filepath.c_str()))
			{
				library_files_used.push_back(resource_1_filepath);
			}
		}

		App->json->UnloadJSON(doc);
	}

	return ret;
}

bool ResourceScriptLoader::ImportAssetFromLibrary(DecomposedFilePath d_filepath, std::vector<Resource*>& resources)
{
	bool ret = false;

	std::string assets_meta_path = d_filepath.file_path + ".meta";

	JSON_Doc* doc = App->json->LoadJSON(assets_meta_path.c_str());

	if (doc != nullptr)
	{
		std::string uid = doc->GetString("resource");
		std::string resource = uid + ".cs";

		std::string resource_filepath = library_path + resource;
		std::string dll_filepath = library_path + ".dll";

		if (App->file_system->FileExists(resource_filepath.c_str()))
		{
			ResourceScript* rs = (ResourceScript*)App->resource->CreateResource(ResourceType::SCRIPT);

			rs->SetData(resource_filepath.c_str(), dll_filepath.c_str());

			ret = true;
		}

		App->json->UnloadJSON(doc);
	}

	return ret;
}

bool ResourceScriptLoader::ExportResourceToLibrary(Resource * resource)
{
	bool ret = false;

	if (resource != nullptr)
	{
		ResourceScript* resource_scr = (ResourceScript*)resource;

		if (App->scripting->compiler->CreateScript(resource_scr->GetScriptPath(), resource_scr->GetScriptCode()))
		{
			App->scripting->compiler->CompileScript(resource_scr->GetScriptPath(), resource_scr->GetScripDLLPath());

			ret = true;
		}
	}

	return ret;
}

void ResourceScriptLoader::OnDestroyResource(Resource * res)
{
}
