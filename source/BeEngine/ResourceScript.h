#ifndef __RESOURCE_SCRIPT_H__
#define __RESOURCE_SCRIPT_H__

#include "Resource.h"
#include "Globals.h"
#include <vector>
#include "ModuleScripting.h"
#include "ScriptingCluster.h"

class ResourceScript;
class GameObject;
class GameObjectComponent;

class ResourceScriptField
{
	friend class ResourceScript;

public:
	ResourceScriptField(ScriptFieldType type, const std::string& field_name);

	ScriptFieldType GetType() const;
	std::string GetName() const;

private:
	ScriptFieldType type = ScriptFieldType::SCRIPT_FIELD_UNDEFINED;
	std::string name;
};

class ResourceScript : public Resource
{
	friend class ResourceScriptLoader;
	friend class ModuleResource;
	friend class ModuleAssets;

private:
	void operator delete(void *) {}

public:
	ResourceScript();

	void CleanUp();

	bool ExistsOnLibrary(std::string uid, std::string& library_filepath = std::string());
	void ExportToLibrary(std::string uid);
	void ImportFromLibrary();

	void OnRemoveAsset();
	void OnRenameAsset(const char* new_name, const char* last_name);
	void OnMoveAsset(const char* new_asset_path, const char* last_asset_path);

	void ClearScriptFields();
	void AddScriptField(const std::string& field_name, const std::string& type_name);
	std::vector<ResourceScriptField> GetFields();

	bool GetInheritsFromBeengineScript() const;
	std::string GetScriptingClassName() const;
	ScriptingClass GetScriptingClass() const;

	bool DrawEditorExplorer();
	void DrawEditorInspector();

private:
	std::string script_class_name;

	std::string script_code;

	bool compiles = false;

	std::vector<std::string> compile_errors;

	bool inherits_from_component_script = false;

	std::vector<ResourceScriptField> script_fields;

	ScriptingClass script_class;
};

#endif // !__RESOURCE_SCRIPT_H__