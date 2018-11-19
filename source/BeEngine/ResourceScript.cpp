#include "ResourceScript.h"
#include "App.h"
#include "ModuleResource.h"
#include "ModuleFileSystem.h"
#include "ModuleScripting.h"
#include "ScriptingObjectCompiler.h"
#include "ScriptingObjectSolutionManager.h"
#include "Functions.h"
#include "ModuleJson.h"

ResourceScript::ResourceScript() : Resource(ResourceType::SCRIPT)
{
}

void ResourceScript::CleanUp()
{

}

bool ResourceScript::ExistsOnLibrary(std::string uid, std::string & library_filepath)
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

void ResourceScript::ExportToLibrary(std::string uid)
{
	Compile();

	App->scripting->solution_manager->AddScript(GetAssetFilepath().c_str());
}

void ResourceScript::ImportFromLibrary()
{
	App->scripting->solution_manager->AddScript(GetAssetFilepath().c_str());

	if(GetCodeDifferentFromMeta())
		Compile();
}

void ResourceScript::OnRemoveAsset()
{
	App->scripting->solution_manager->RemoveScript(GetAssetFilepath().c_str());
}

void ResourceScript::OnRenameAsset(const char * new_name, const char * last_name)
{
	std::string code = App->scripting->compiler->GetScriptCode(GetAssetFilepath().c_str());

	code = TextReplace(code, last_name, new_name);

	App->scripting->compiler->SetScriptCode(GetAssetFilepath().c_str(), code.c_str());

	Compile();

	DecomposedFilePath dfp = App->file_system->DecomposeFilePath(GetAssetFilepath().c_str());
	std::string last_path = dfp.path + last_name + "." + dfp.file_extension;
	App->scripting->solution_manager->RemoveScript(last_path.c_str());
	App->scripting->solution_manager->AddScript(GetAssetFilepath().c_str());
}

void ResourceScript::Compile()
{
	std::string library_path = App->resource->GetLibraryPathFromResourceType(GetType());

	std::string dll_output_path = library_path + GetUID() + ".dll";

	if (App->file_system->FileExists(dll_output_path.c_str()))
		App->file_system->FileDelete(dll_output_path.c_str());

	compiles = App->scripting->compiler->CompileScript(GetAssetFilepath().c_str(), dll_output_path.c_str(), compile_errors);

	SaveCodeOnMeta();

	if (!compiles)
	{
		for (std::vector<std::string>::iterator it = compile_errors.begin(); it != compile_errors.end(); ++it)
		{
			CONSOLE_ERROR((*it).c_str());
		}
	}
}

void ResourceScript::SaveCodeOnMeta()
{
	JSON_Doc* doc = App->json->LoadJSON(GetMetaFilepath().c_str());

	if (doc != nullptr)
	{
		std::string code = App->scripting->compiler->GetScriptCode(GetAssetFilepath().c_str());

		doc->SetString("code", code.c_str());

		doc->Save();

		App->json->UnloadJSON(doc);
	}
}

bool ResourceScript::GetCodeDifferentFromMeta()
{
	bool ret = false;

	JSON_Doc* doc = App->json->LoadJSON(GetMetaFilepath().c_str());

	if (doc != nullptr)
	{
		std::string code = App->scripting->compiler->GetScriptCode(GetAssetFilepath().c_str());

		std::string meta_code = doc->GetString("code");

		if (meta_code.compare(code) != 0)
			ret = true;

		App->json->UnloadJSON(doc);
	}

	return ret;
}
