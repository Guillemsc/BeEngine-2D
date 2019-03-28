#include "ComponentScriptFields.h"
#include "ScriptingCluster.h"

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

MonoObject * ComponentScriptFieldInt::BoxObject()
{
	return nullptr;
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

MonoObject * ComponentScriptFieldFloat::BoxObject()
{
	return nullptr;
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

MonoObject * ComponentScriptFieldString::BoxObject()
{
	return nullptr;
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

MonoObject * ComponentScriptFieldBool::BoxObject()
{
	return nullptr;
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

MonoObject * ComponentScriptFieldGameObject::BoxObject()
{
	return nullptr;
}
