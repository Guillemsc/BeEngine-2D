#ifndef __COMPONENT_SCRIPT_H__
#define __COMPONENT_SCRIPT_H__

#include "GameObjectComponent.h"
#include "ResourceSelectorWidget.h"
#include "GeometryMath.h"
#include "GameObjectSelectorWidget.h"

class GameObject;
class ResourceScript;
class Event;
class ResourceScriptField;
class ComponentScriptField;
class ScriptingBridgeComponentScript;

class ComponentScript : public GameObjectComponent
{
	friend class GameObject;
	friend class ModuleGameObject;

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

	void UpdateScriptInstance();

	void InitFields();
	bool CallAwake();
	bool CallStart();
	void CallUpdate();
	void CallOnDestroy();

private:
	void DrawFieldValue(ComponentScriptField* field_value);

	void RecalculateFieldsValues(const std::vector<ResourceScriptField>& fields);
	void RemoveFieldValues(const std::vector<ComponentScriptField*>& vals);
	void RemoveAllFieldValues();

	void CheckFieldGameObjectDestroyed(GameObject* go);
	void CheckFieldResourceDestroyed(Resource* res);

private:
	ResourceScript* resource_script = nullptr;

	ResourceSelectorWidget resource_selector_widget;
	GameObjectSelectorWidget go_selector_widget;

	std::vector<ComponentScriptField*> fields_values;

	bool needs_init = true;
};

#endif // !__COMPONENT_SCRIPT_H__