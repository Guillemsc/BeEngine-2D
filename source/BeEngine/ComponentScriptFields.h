#ifndef __COMPONENT_SCRIPT_FIELDS_H__
#define __COMPONENT_SCRIPT_FIELDS_H__

#include "GameObjectComponent.h"
#include "GeometryMath.h"

class GameObject;
class ResourceScript;
class Event;
class ResourceScriptFieldValue;
class ResourceScriptField;
class GameObject;

enum ScriptFieldType;

/*	SCRIPT_FIELD_UNDEFINED,
	SCRIPT_FIELD_GAMEOBJECT,*/

class ComponentScriptField
{
public:
	ComponentScriptField(ScriptFieldType type, const std::string& name);

	ScriptFieldType GetType() const;
	std::string GetName() const;

	virtual MonoObject* BoxObject() {};

private:
	ScriptFieldType type;
	std::string name;
};

class ComponentScriptFieldInt : public ComponentScriptField
{
public:
	ComponentScriptFieldInt(const std::string& name);

	void SetValue(int val);
	int GetValue() const;

	MonoObject* BoxObject();

private:
	int value = 0;
};

class ComponentScriptFieldFloat : public ComponentScriptField
{
public:
	ComponentScriptFieldFloat(const std::string& name);

	void SetValue(float val);
	float GetValue() const;

	MonoObject* BoxObject();

private:
	float value = 0;
};

class ComponentScriptFieldString : public ComponentScriptField
{
public:
	ComponentScriptFieldString(const std::string& name);

	void SetValue(const std::string& val);
	std::string GetValue() const;

	MonoObject* BoxObject();

private:
	std::string value = "";
};


class ComponentScriptFieldBool : public ComponentScriptField
{
public:
	ComponentScriptFieldBool(const std::string& name);

	void SetValue(bool val);
	bool GetValue() const;

	MonoObject* BoxObject();

private:
	bool value = false;
};

class ComponentScriptFieldGameObject : public ComponentScriptField
{
public:
	ComponentScriptFieldGameObject(const std::string& name);

	void SetValue(GameObject* val);
	GameObject* GetValue() const;

	MonoObject* BoxObject();

private:
	GameObject* value = false;
};

#endif // !__COMPONENT_SCRIPT_FIELDS_H__
