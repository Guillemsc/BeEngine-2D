#ifndef __COMPONENT_SCRIPT_FIELDS_H__
#define __COMPONENT_SCRIPT_FIELDS_H__

#include "GameObjectComponent.h"
#include "GeometryMath.h"
#include "ModuleScripting.h"

class GameObject;
class ResourceScript;
class Event;
class ResourceScriptFieldValue;
class ResourceScriptField;
class ResourcePrefab;
class ResourceScene;

enum ScriptFieldType;

class ComponentScriptField
{
public:
	ComponentScriptField(ScriptFieldType type, const std::string& name);

	ScriptFieldType GetType() const;
	std::string GetName() const;

	virtual void* VoidValue() { return nullptr; }

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

	void* VoidValue();

private:
	int value = 0;
};

class ComponentScriptFieldFloat : public ComponentScriptField
{
public:
	ComponentScriptFieldFloat(const std::string& name);

	void SetValue(float val);
	float GetValue() const;

	void* VoidValue();

private:
	float value = 0;
};

class ComponentScriptFieldString : public ComponentScriptField
{
public:
	ComponentScriptFieldString(const std::string& name);

	void SetValue(const std::string& val);
	std::string GetValue() const;

	void* VoidValue();

private:
	std::string value = "";
};


class ComponentScriptFieldBool : public ComponentScriptField
{
public:
	ComponentScriptFieldBool(const std::string& name);

	void SetValue(bool val);
	bool GetValue() const;

	void* VoidValue();

private:
	bool value = false;
};

class ComponentScriptFieldGameObject : public ComponentScriptField
{
public:
	ComponentScriptFieldGameObject(const std::string& name);

	void SetValue(GameObject* val);
	GameObject* GetValue() const;

	void* VoidValue();

private:
	GameObject* value = false;
};

class ComponentScriptFieldResourcePrefab : public ComponentScriptField
{
public:
	ComponentScriptFieldResourcePrefab(const std::string& name);

	void SetValue(ResourcePrefab* val);
	ResourcePrefab* GetValue() const;

	void* VoidValue();

private:
	ResourcePrefab* value = false;
};

class ComponentScriptFieldResourceScene : public ComponentScriptField
{
public:
	ComponentScriptFieldResourceScene(const std::string& name);

	void SetValue(ResourceScene* val);
	ResourceScene* GetValue() const;

	void* VoidValue();

private:
	ResourceScene* value = false;
};



#endif // !__COMPONENT_SCRIPT_FIELDS_H__
