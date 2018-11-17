#include "ResourceScript.h"
#include "App.h"
#include "ModuleResource.h"
#include "ModuleFileSystem.h"
#include "ModuleScripting.h"
#include "ScriptingObjectCompiler.h"
#include "ScriptingObjectSolutionManager.h"

ResourceScript::ResourceScript() : Resource(ResourceType::SCRIPT)
{
}

void ResourceScript::CleanUp()
{

}

bool ResourceScript::OnExistsOnLibrary(std::string uid, std::string & library_filepath)
{
	bool ret = false;

	std::string library_path = App->resource->GetLibraryPathFromResourceType(GetType());

	std::string path = library_path + uid + ".dll";

	if (App->file_system->FileExists(path.c_str()))
	{
		library_filepath = path;
		ret = true;
	}

	return ret;
}

void ResourceScript::OnExportToLibrary(std::string uid)
{
	std::string library_path = App->resource->GetLibraryPathFromResourceType(GetType());

	std::string dll_output_path = library_path + uid + ".dll";

	compiles = App->scripting->compiler->CompileScript(GetAssetFilepath().c_str(), dll_output_path.c_str(), compile_errors);

	App->scripting->solution_manager->AddScript(GetAssetFilepath().c_str());
}

void ResourceScript::OnImportFromLibrary()
{
	App->scripting->solution_manager->AddScript(GetAssetFilepath().c_str());
}

void ResourceScript::OnRemoveAsset()
{
	App->scripting->solution_manager->RemoveScript(GetAssetFilepath().c_str());
}
