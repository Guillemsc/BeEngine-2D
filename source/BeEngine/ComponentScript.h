#ifndef __COMPONENT_SCRIPT_H__
#define __COMPONENT_SCRIPT_H__

#include "GameObjectComponent.h"

#include "GeometryMath.h"

class GameObject;
class ResourceScript;
class Event;
class ResourceScriptFieldValue;
class ResourceScriptField;
class ScriptingBridgeComponentScript;

class ComponentScript : public GameObjectComponent
{
	friend class GameObject;

private:
	void operator delete(void *) {}

public:
	ComponentScript();
	~ComponentScript();

	void EditorDraw();

	void Start();
	void CleanUp();

	void OnSaveAbstraction(DataAbstraction& abs);
	void OnLoadAbstraction(DataAbstraction& abs);

	void OnEvent(Event* ev);

	void OnChildAdded(GameObject* child);
	void OnChildRemoved(GameObject* child);
	void OnParentChanged(GameObject* new_parent);

	void SetResourceScript(ResourceScript* set);

	void CreateScriptInstance();
	void DestroyScriptInstance();

	void CallAwake();
	void CallStart();
	void CallUpdate();
	void CallOnDestroy();

private:
	void DrawFieldValue(ResourceScriptFieldValue& field_value);

	void ClearFieldsValues();
	void RecalculateFieldsValues(std::vector<ResourceScriptField> fields);

private:
	ResourceScript* resource_script = nullptr;

	std::string resource_filter;

	std::vector<ResourceScriptFieldValue> fields_values;

	ScriptingBridgeComponentScript* scripting_bridge = nullptr;
};

#endif // !__COMPONENT_SCRIPT_H__