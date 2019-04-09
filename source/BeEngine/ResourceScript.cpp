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

	std::string library_path = App->resource->GetLibraryPathFromResourceType(GetType());

	std::string path = library_path + uid + ".bescript";

	if (App->file_system->FileExists(path.c_str()))
	{
		library_filepath = path;
		ret = true;
	}

	return ret;
}

void ResourceScript::ExportToLibrary(std::string uid)
{
	App->scripting->compiler->AddScript(GetAssetFilepath().c_str());
	App->scripting->solution_manager->AddScript(GetAssetFilepath().c_str());

	std::string library_path = App->resource->GetLibraryPathFromResourceType(GetType());

	std::string filepath = library_path + uid + ".bescript";

	if (App->file_system->FileExists(filepath))
		App->file_system->FileDelete(filepath);
	
	JSON_Doc* doc = App->json->CreateJSON(filepath.c_str());
	if (doc != nullptr)
	{
		doc->SetString("name", d_asset_filepath.file_name.c_str());
		doc->Save();

		App->json->UnloadJSON(doc);
	}
}

void ResourceScript::ImportFromLibrary()
{
	App->scripting->solution_manager->AddScript(GetAssetFilepath().c_str());
	App->scripting->compiler->AddScript(GetAssetFilepath().c_str());

	std::string resource_filepath = GetLibraryFilepath();

	JSON_Doc* doc = App->json->LoadJSON(resource_filepath.c_str());

	if (doc != nullptr)
	{
		script_class_name = doc->GetString("name");

		App->json->UnloadJSON(doc);
	}

	script_code = App->scripting->compiler->GetScriptCode(GetAssetFilepath().c_str());
}

void ResourceScript::OnRemoveAsset()
{
	App->scripting->solution_manager->RemoveScript(GetAssetFilepath().c_str());
	App->scripting->compiler->RemoveScript(GetAssetFilepath().c_str());

	App->scripting->CompileScripts();
}

void ResourceScript::OnRenameAsset(const char * new_name, const char * last_name)
{
	std::string resource_filepath = GetLibraryFilepath();

	JSON_Doc* doc = App->json->LoadJSON(resource_filepath.c_str());

	if (doc == nullptr)
		doc = App->json->CreateJSON(resource_filepath.c_str());

	if (doc != nullptr)
	{
		doc->SetString("name", new_name);
		doc->Save();

		App->json->UnloadJSON(doc);
	}

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

std::string ResourceScript::GetScriptingClassName() const
{
	return script_class_name;
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

void ResourceScript::DrawEditorInspector()
{
	ImGui::TextWrapped(script_code.c_str());
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
