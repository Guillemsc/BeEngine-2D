#ifndef __RESOURCE_SCRIPT_H__
#define __RESOURCE_SCRIPT_H__

#include "Resource.h"
#include "Globals.h"
#include <vector>
#include "ModuleScripting.h"

class ResourceScript;
class GameObject;
class GameObjectComponent;

enum ResourceScriptFieldType
{
	SCRIPT_FIELD_UNDEFINED,
	SCRIPT_FIELD_INT,
	SCRIPT_FIELD_FLOAT,
	SCRIPT_FIELD_STRING,
	SCRIPT_FIELD_BOOL,
	//SCRIPT_FIELD_GAMEOBJECT,
	//SCRIPT_FIELD_GAMEOBJECT_COMPONENT,
	//SCRIPT_FIELD_GAMEOBJECT_RESOURCE,
};

class ResourceScriptField
{
	friend class ResourceScript;

public:
	ResourceScriptField();

	ResourceScriptFieldType type = ResourceScriptFieldType::SCRIPT_FIELD_UNDEFINED;
	std::string field_name;
};

class ResourceScriptFieldValue
{
	friend class ResourceScript;

public:
	ResourceScriptFieldValue(const ResourceScriptField field);

	std::string GetFieldName() const;
	ResourceScriptFieldType GetFieldType() const;

public:
	int int_field = 0;
	float float_field = 0.0f;
	std::string string_field = "";
	bool bool_field = false;
	//GameObject* game_object_field = nullptr;
	//GameObjectComponent* game_object_class_field = nullptr;
	//Resource* resource_field = nullptr;

private:
	ResourceScriptField field;
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
	void AddIntScriptField(const std::string& field_name);
	void AddFloatScriptField(const std::string& field_name);
	void AddStringScriptField(const std::string& field_name);
	void AddBoolScriptField(const std::string& field_name);
	std::vector<ResourceScriptField> GetFields();

	bool GetInheritsFromBeengineReference() const;
	ScriptingClass GetScriptingClass() const;

private:
	bool compiles = false;

	std::vector<std::string> compile_errors;

	bool inherits_from_beengine_reference = false;

	std::vector<ResourceScriptField> script_fields;

	ScriptingClass script_class;
};

#endif // !__RESOURCE_SCRIPT_H__