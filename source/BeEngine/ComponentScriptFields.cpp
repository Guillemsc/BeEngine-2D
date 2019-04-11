#include "ComponentScriptFields.h"
#include "ScriptingCluster.h"
#include "App.h"
#include "GameObject.h"
#include "ScriptingBridgeGameObject.h"
#include "ResourcePrefab.h"
#include "ResourceScene.h"

ComponentScriptField::ComponentScriptField(ScriptFieldType t, const std::string & n)
{
	type = t;
	name = n;
}

ScriptFieldType ComponentScriptField::GetType() const
{
	return type;
}

std::string ComponentScriptField::GetName() const
{
	return name;
}

ComponentScriptFieldInt::ComponentScriptFieldInt(const std::string& name) 
	: ComponentScriptField(ScriptFieldType::SCRIPT_FIELD_INT, name)
{
}

void ComponentScriptFieldInt::SetValue(int val)
{
	value = val;
}

int ComponentScriptFieldInt::GetValue() const
{
	return value;
}

void * ComponentScriptFieldInt::VoidValue()
{
	return &value;
}

ComponentScriptFieldFloat::ComponentScriptFieldFloat(const std::string & name)
	: ComponentScriptField(ScriptFieldType::SCRIPT_FIELD_FLOAT, name)
{
}

void ComponentScriptFieldFloat::SetValue(float val)
{
	value = val;
}

float ComponentScriptFieldFloat::GetValue() const
{
	return value;
}

void * ComponentScriptFieldFloat::VoidValue()
{
	return &value;
}

ComponentScriptFieldString::ComponentScriptFieldString(const std::string & name)
	: ComponentScriptField(ScriptFieldType::SCRIPT_FIELD_STRING, name)
{
}

void ComponentScriptFieldString::SetValue(const std::string & val)
{
	value = val;
}

std::string ComponentScriptFieldString::GetValue() const
{
	return value;
}

void * ComponentScriptFieldString::VoidValue()
{
	return App->scripting->BoxString(value.c_str());
}

ComponentScriptFieldBool::ComponentScriptFieldBool(const std::string & name)
	: ComponentScriptField(ScriptFieldType::SCRIPT_FIELD_BOOL, name)
{
}

void ComponentScriptFieldBool::SetValue(bool val)
{
	value = val;
}

bool ComponentScriptFieldBool::GetValue() const
{
	return value;
}

void * ComponentScriptFieldBool::VoidValue()
{
	return &value;
}

ComponentScriptFieldGameObject::ComponentScriptFieldGameObject(const std::string & name)
	: ComponentScriptField(ScriptFieldType::SCRIPT_FIELD_GAMEOBJECT, name)
{
}

void ComponentScriptFieldGameObject::SetValue(GameObject * val)
{
	value = val;
}

GameObject * ComponentScriptFieldGameObject::GetValue() const
{
	return value;
}

void * ComponentScriptFieldGameObject::VoidValue()
{
	if (value != nullptr)
		return value->GetScriptingBridge()->GetInstance()->GetMonoObject();

	return nullptr;
}

ComponentScriptFieldResourcePrefab::ComponentScriptFieldResourcePrefab(const std::string & name)
	: ComponentScriptField(ScriptFieldType::SCRIPT_FIELD_RESOURCE_PREFAB, name)
{
}

void ComponentScriptFieldResourcePrefab::SetValue(ResourcePrefab * val)
{
	value = val;
}

ResourcePrefab * ComponentScriptFieldResourcePrefab::GetValue() const
{
	return value;
}

void * ComponentScriptFieldResourcePrefab::VoidValue()
{
	if (value != nullptr)
		return value->GetScriptingBridge()->GetInstance()->GetMonoObject();

	return nullptr;
}

ComponentScriptFieldResourceScene::ComponentScriptFieldResourceScene(const std::string & name)
	: ComponentScriptField(ScriptFieldType::SCRIPT_FIELD_RESOURCE_SCENE, name)
{
}

void ComponentScriptFieldResourceScene::SetValue(ResourceScene * val)
{
	value = val;
}

ResourceScene * ComponentScriptFieldResourceScene::GetValue() const
{
	return value;
}

void * ComponentScriptFieldResourceScene::VoidValue()
{
	if (value != nullptr)
		return value->GetScriptingBridge()->GetInstance()->GetMonoObject();

	return nullptr;
}
