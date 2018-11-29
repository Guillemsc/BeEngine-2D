#include "ResourceScript.h"
#include "App.h"
#include "ModuleResource.h"
#include "ModuleFileSystem.h"
#include "ModuleScripting.h"
#include "ScriptingObjectCompiler.h"
#include "ScriptingObjectSolutionManager.h"
#include "Functions.h"
#include "ModuleJson.h"

ResourceScript::ResourceScript() : Resource(ResourceType::RESOURCE_TYPE_SCRIPT)
{
}

void ResourceScript::CleanUp()
{

}

bool ResourceScript::ExistsOnLibrary(std::string uid, std::string & library_filepath)
{
	bool ret = false;

	return true;

	return ret;
}

void ResourceScript::ExportToLibrary(std::string uid)
{
	App->scripting->compiler->AddScript(GetAssetFilepath().c_str());
	App->scripting->solution_manager->AddScript(GetAssetFilepath().c_str());
}

void ResourceScript::ImportFromLibrary()
{
	App->scripting->solution_manager->AddScript(GetAssetFilepath().c_str());
	App->scripting->compiler->AddScript(GetAssetFilepath().c_str());
}

void ResourceScript::OnRemoveAsset()
{
	App->scripting->solution_manager->RemoveScript(GetAssetFilepath().c_str());
	App->scripting->compiler->RemoveScript(GetAssetFilepath().c_str());

	App->scripting->CompileScripts();
}

void ResourceScript::OnRenameAsset(const char * new_name, const char * last_name)
{
	std::string code = App->scripting->compiler->GetScriptCode(GetAssetFilepath().c_str());

	code = TextReplace(code, last_name, new_name);

	App->scripting->compiler->SetScriptCode(GetAssetFilepath().c_str(), code.c_str());

	DecomposedFilePath dfp = App->file_system->DecomposeFilePath(GetAssetFilepath().c_str());
	std::string last_path = dfp.path + last_name + "." + dfp.file_extension;

	App->scripting->compiler->AddScript(GetAssetFilepath().c_str());
	App->scripting->compiler->RemoveScript(last_path.c_str());

	App->scripting->solution_manager->RemoveScript(last_path.c_str());
	App->scripting->solution_manager->AddScript(GetAssetFilepath().c_str());

	App->scripting->CompileScripts();
}

void ResourceScript::OnMoveAsset(const char * new_asset_path, const char* last_asset_path)
{
	App->scripting->solution_manager->RemoveScript(last_asset_path);
	App->scripting->compiler->RemoveScript(last_asset_path);

	App->scripting->compiler->AddScript(new_asset_path);
	App->scripting->solution_manager->AddScript(new_asset_path);

	App->scripting->CompileScripts();
}

void ResourceScript::ClearScriptFields()
{
	script_fields.clear();
}

void ResourceScript::AddIntScriptField(const std::string & field_name)
{
	ResourceScriptField sf;
	sf.type = ResourceScriptFieldType::SCRIPT_FIELD_INT;
	sf.field_name = field_name;

	script_fields.push_back(sf);
}

void ResourceScript::AddFloatScriptField(const std::string & field_name)
{
	ResourceScriptField sf;
	sf.type = ResourceScriptFieldType::SCRIPT_FIELD_FLOAT;
	sf.field_name = field_name;

	script_fields.push_back(sf);
}

void ResourceScript::AddStringScriptField(const std::string & field_name)
{
	ResourceScriptField sf;
	sf.type = ResourceScriptFieldType::SCRIPT_FIELD_STRING;
	sf.field_name = field_name;

	script_fields.push_back(sf);
}

void ResourceScript::AddBoolScriptField(const std::string & field_name)
{
	ResourceScriptField sf;
	sf.type = ResourceScriptFieldType::SCRIPT_FIELD_BOOL;
	sf.field_name = field_name;

	script_fields.push_back(sf);
}

ResourceScriptField::ResourceScriptField()
{

}
