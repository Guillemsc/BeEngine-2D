#include "ResourceScript.h"
#include "App.h"
#include "ModuleResource.h"
#include "ModuleFileSystem.h"
#include "ScriptingObjectCompiler.h"
#include "ScriptingObjectSolutionManager.h"
#include "Functions.h"
#include "ModuleJson.h"
#include "imgui.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

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

void ResourceScript::AddScriptField(const std::string & field_name, const std::string& type_name)
{
	ScriptFieldType type = App->scripting->scripting_cluster->GetScriptFieldTypeFromName(type_name);

	if (type != ScriptFieldType::SCRIPT_FIELD_UNDEFINED)
	{
		ResourceScriptField sf(type, field_name);
		script_fields.push_back(sf);
	}
}

std::vector<ResourceScriptField> ResourceScript::GetFields()
{
	return script_fields;
}

bool ResourceScript::GetInheritsFromBeengineScript() const
{
	return inherits_from_component_script;
}

ScriptingClass ResourceScript::GetScriptingClass() const
{
	return script_class;
}

bool ResourceScript::DrawEditorExplorer()
{
	bool ret = false;

	if (ImGui::Button("Edit Script"))
	{
		App->scripting->solution_manager->OpenSolutionWithExternalProgram();

		ret = true;
	}

	return ret;
}

ResourceScriptField::ResourceScriptField(ScriptFieldType t, const std::string& f)
{
	type = t;
	name = f;
}

ScriptFieldType ResourceScriptField::GetType() const
{
	return type;
}

std::string ResourceScriptField::GetName() const
{
	return name;
}
