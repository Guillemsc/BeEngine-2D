#include "ComponentScript.h"
#include "imgui.h"
#include "App.h"
#include "ModuleResource.h"
#include "ResourceScript.h"
#include "ModuleEvent.h"
#include "Event.h"
#include "Functions.h"
#include "ModuleScripting.h"
#include "GameObject.h"
#include "ComponentTransfrom.h"
#include "ModuleJson.h"
#include "ScriptingBridgeComponentScript.h"
#include "ComponentScriptFields.h"
#include "ModuleGameObject.h"
#include "ModuleState.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

ComponentScript::ComponentScript() 
	: GameObjectComponent(new ScriptingBridgeComponentScript(this),
		"Script", ComponentType::COMPONENT_TYPE_SCRIPT, ComponentGroup::SCRIPTING)
{
	
}

ComponentScript::~ComponentScript()
{
}

void ComponentScript::EditorDraw()
{
	bool open_error_script = false;

	Resource* res = resource_script;
	if (resource_selector_widget.Draw("Script:", ResourceType::RESOURCE_TYPE_SCRIPT, res))
	{
		ResourceScript* resource = (ResourceScript*)res;
		if (resource != nullptr && !resource->GetInheritsFromBeengineScript())
		{
			open_error_script = true;
		}
		else
		{
			resource_script = (ResourceScript*)res;

			if (resource_script != nullptr)
			{
				RecalculateFieldsValues(resource_script->GetFields());
			}
		}

		UpdateScriptInstance();
	}

	if (open_error_script)
	{
		ImGui::OpenPopup("ErrorSelectingScript");
	}

	if (ImGui::BeginPopup("ErrorSelectingScript"))
	{
		ImGui::Text("Error adding script!");
		ImGui::Separator();
		ImGui::Text("Check that the script class has the same");
		ImGui::Text("name as the script file. Also, the class");
		ImGui::Text("needs to derive from BeEngineScript.");

		ImGui::Spacing();
		if (ImGui::Button("Ok", ImVec2(230, 0)))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (resource_script != nullptr)
	{
		if (fields_values.size() > 0)
			ImGui::Separator();

		for (std::vector<ComponentScriptField*>::iterator it = fields_values.begin(); it != fields_values.end(); ++it)
		{
			DrawFieldValue((*it));
		}
	}
}

void ComponentScript::Start()
{
	App->event->Suscribe(std::bind(&ComponentScript::OnEvent, this, std::placeholders::_1), EventType::RESOURCE_DESTROYED);
	App->event->Suscribe(std::bind(&ComponentScript::OnEvent, this, std::placeholders::_1), EventType::GAME_OBJECT_DESTROYED);
	App->event->Suscribe(std::bind(&ComponentScript::OnEvent, this, std::placeholders::_1), EventType::RESOURCE_SCRIPTS_FIELDS_CHANGED);

	App->gameobject->AddComponentScript(this);
}

void ComponentScript::CleanUp()
{
	App->gameobject->RemoveComponentScript(this);

	RemoveAllFieldValues();

	App->event->UnSuscribe(std::bind(&ComponentScript::OnEvent, this, std::placeholders::_1), EventType::RESOURCE_DESTROYED);
	App->event->UnSuscribe(std::bind(&ComponentScript::OnEvent, this, std::placeholders::_1), EventType::GAME_OBJECT_DESTROYED);
	App->event->UnSuscribe(std::bind(&ComponentScript::OnEvent, this, std::placeholders::_1), EventType::RESOURCE_SCRIPTS_FIELDS_CHANGED);
}

void ComponentScript::OnSaveAbstraction(DataAbstraction & abs)
{
	int fields_count = fields_values.size();

	abs.AddInt("fields_count", fields_count);

	int counter = 0;
	for (std::vector<ComponentScriptField*>::iterator it = fields_values.begin(); it != fields_values.end(); ++it, ++counter)
	{
		std::string number = std::to_string(counter);

		std::string field_name_name = "field_name_" + number;
		std::string field_type_name = "field_type_" + number;
		std::string field_val_name = "field_val_" + number;

		abs.AddString(field_name_name, (*it)->GetName());
		abs.AddInt(field_type_name, (*it)->GetType());

		switch ((*it)->GetType())
		{
			case ScriptFieldType::SCRIPT_FIELD_INT:
			{
				ComponentScriptFieldInt* field = (ComponentScriptFieldInt*)(*it);

				abs.AddInt(field_val_name, field->GetValue());
				break;
			}

			case ScriptFieldType::SCRIPT_FIELD_FLOAT:
			{
				ComponentScriptFieldFloat* field = (ComponentScriptFieldFloat*)(*it);

				abs.AddFloat(field_val_name, field->GetValue());
				break;
			}

			case ScriptFieldType::SCRIPT_FIELD_STRING:
			{
				ComponentScriptFieldString* field = (ComponentScriptFieldString*)(*it);

				abs.AddString(field_val_name, field->GetValue());
				break;
			}

			case ScriptFieldType::SCRIPT_FIELD_BOOL:
			{
				ComponentScriptFieldBool* field = (ComponentScriptFieldBool*)(*it);

				abs.AddBool(field_val_name, field->GetValue());
				break;
			}

			case ScriptFieldType::SCRIPT_FIELD_GAMEOBJECT:
			{
				ComponentScriptFieldGameObject* field = (ComponentScriptFieldGameObject*)(*it);

				std::string uid = "";

				GameObject* go_to_save = field->GetValue();

				if (go_to_save != nullptr)
					uid = go_to_save->GetUID();

				abs.AddString(field_val_name, uid);
				break;
			}

			case ScriptFieldType::SCRIPT_FIELD_RESOURCE_PREFAB:
			{
				ComponentScriptFieldResourcePrefab* field = (ComponentScriptFieldResourcePrefab*)(*it);

				std::string uid = "";

				Resource* res_to_save = (Resource*)field->GetValue();

				if (res_to_save != nullptr)
					uid = res_to_save->GetUID();

				abs.AddString(field_val_name, uid);
				break;
			}

			case ScriptFieldType::SCRIPT_FIELD_RESOURCE_SCENE:
			{
				ComponentScriptFieldResourceScene* field = (ComponentScriptFieldResourceScene*)(*it);

				std::string uid = "";

				Resource* res_to_save = (Resource*)field->GetValue();

				if (res_to_save != nullptr)
					uid = res_to_save->GetUID();

				abs.AddString(field_val_name, uid);
				break;
			}

		default:
			break;
		}
	}

	if (resource_script != nullptr)
		abs.AddString("resource", resource_script->GetUID());
}

void ComponentScript::OnLoadAbstraction(DataAbstraction & abs)
{
	RemoveAllFieldValues();

	int fields_count = abs.GetInt("fields_count");

	for (int i = 0; i < fields_count; ++i)
	{
		std::string number = std::to_string(i);

		std::string field_name_name = "field_name_" + number;
		std::string field_type_name = "field_type_" + number;
		std::string field_val_name = "field_val_" + number;

		std::string name = abs.GetString(field_name_name);
		int field_int = abs.GetInt(field_type_name, -1);

		if (field_int > -1)
		{
			ScriptFieldType field = static_cast<ScriptFieldType>(field_int);

			ComponentScriptField* field_obj = nullptr;

			switch (field)
			{
			case ScriptFieldType::SCRIPT_FIELD_INT:
			{
				int val = abs.GetInt(field_val_name);
				field_obj = new ComponentScriptFieldInt(name);
				((ComponentScriptFieldInt*)field_obj)->SetValue(val);
				break;
			}

			case ScriptFieldType::SCRIPT_FIELD_FLOAT:
			{
				float val = abs.GetFloat(field_val_name);
				field_obj = new ComponentScriptFieldFloat(name);
				((ComponentScriptFieldFloat*)field_obj)->SetValue(val);
				break;
			}

			case ScriptFieldType::SCRIPT_FIELD_STRING:
			{
				std::string val = abs.GetString(field_val_name);
				field_obj = new ComponentScriptFieldString(name);
				((ComponentScriptFieldString*)field_obj)->SetValue(val);
				break;
			}

			case ScriptFieldType::SCRIPT_FIELD_BOOL:
			{
				bool val = abs.GetBool(field_val_name);
				field_obj = new ComponentScriptFieldBool(name);
				((ComponentScriptFieldBool*)field_obj)->SetValue(val);
				break;
			}

			case ScriptFieldType::SCRIPT_FIELD_GAMEOBJECT:
			{
				std::string uid = abs.GetString(field_val_name);
				std::string instance_uid = GetOwner()->GetInstanceUID();

				GameObject* value = nullptr;

				if (uid.compare("") != 0)
					value = App->gameobject->GetGameObjectByUID(uid, instance_uid);
				
				field_obj = new ComponentScriptFieldGameObject(name);
				((ComponentScriptFieldGameObject*)field_obj)->SetValue(value);

				break;
			}

			case ScriptFieldType::SCRIPT_FIELD_RESOURCE_PREFAB:
			{
				std::string uid = abs.GetString(field_val_name);

				Resource* value = nullptr;

				if (uid.compare("") != 0)
					value = App->resource->GetResourceFromUid(uid, ResourceType::RESOURCE_TYPE_PREFAB);

				field_obj = new ComponentScriptFieldResourcePrefab(name);
				((ComponentScriptFieldResourcePrefab*)field_obj)->SetValue((ResourcePrefab*)value);

				break;
			}

			case ScriptFieldType::SCRIPT_FIELD_RESOURCE_SCENE:
			{
				std::string uid = abs.GetString(field_val_name);

				Resource* value = nullptr;

				if (uid.compare("") != 0)
					value = App->resource->GetResourceFromUid(uid, ResourceType::RESOURCE_TYPE_SCENE);

				field_obj = new ComponentScriptFieldResourceScene(name);
				((ComponentScriptFieldResourceScene*)field_obj)->SetValue((ResourceScene*)value);

				break;
			}

			}

			if (field_obj != nullptr)
			{
				fields_values.push_back(field_obj);
			}
		}
	}

	std::string resource_uid = abs.GetString("resource");

	if (resource_uid.compare("") != 0)
	{
		resource_script = (ResourceScript*)App->resource->GetResourceFromUid(resource_uid, ResourceType::RESOURCE_TYPE_SCRIPT);

		UpdateScriptInstance();

		if (App->state->GetEngineState() != EngineState::ENGINE_STATE_BUILD)
		{
			if (App->state->GetEditorUpdateState() != EditorUpdateState::EDITOR_UPDATE_STATE_PLAY)
			{
				if (resource_script != nullptr)
				{
					RecalculateFieldsValues(resource_script->GetFields());
				}
			}
		}
	}

	/*App->gameobject->AddComponentScript(this);*/
}

void ComponentScript::OnEvent(Event * ev)
{
	switch (ev->GetType())
	{
	case EventType::RESOURCE_DESTROYED:
	{
		EventResourceDestroyed* erd = (EventResourceDestroyed*)ev;

		if (erd->GetResource() == resource_script)
		{
			resource_script = nullptr;

			RemoveAllFieldValues();

			UpdateScriptInstance();
		}
		else
		{
			CheckFieldResourceDestroyed(erd->GetResource());
		}

		break;
	}

	case EventType::GAME_OBJECT_DESTROYED:
	{
		EventGameObjectDestroyed* egd = (EventGameObjectDestroyed*)ev;

		CheckFieldGameObjectDestroyed(egd->GetGameObject());

		break;
	}

	case EventType::RESOURCE_SCRIPTS_FIELDS_CHANGED:
	{
		if (resource_script != nullptr)
		{
			if (!resource_script->GetInheritsFromBeengineScript())
			{
				resource_script = nullptr;
			}
			else
			{
				RecalculateFieldsValues(resource_script->GetFields());
			}

			UpdateScriptInstance();
		}

		break;
	}
	default:
		break;
	}
}

void ComponentScript::OnChildAdded(GameObject * child)
{
}

void ComponentScript::OnChildRemoved(GameObject * child)
{
}

void ComponentScript::OnParentChanged(GameObject * new_parent)
{
}

void ComponentScript::SetResourceScript(ResourceScript * set)
{
	resource_script = set;
}

void ComponentScript::UpdateScriptInstance()
{
	ScriptingBridgeComponentScript* bridge = (ScriptingBridgeComponentScript*)GetScriptingBridge();

	if (resource_script != nullptr)
	{
		bridge->SetGeneratedClass(resource_script->GetScriptingClass());
		bridge->RebuildInstance();
		bridge->OnRebuildInstances();
	}
	else
	{
		bridge->RemoveGeneratedClass();
		bridge->RebuildInstance();
		bridge->OnRebuildInstances();
	}
}

void ComponentScript::InitFields()
{
	ScriptingBridgeComponentScript* bridge = (ScriptingBridgeComponentScript*)GetScriptingBridge();

	for (std::vector<ComponentScriptField*>::iterator it = fields_values.begin(); it != fields_values.end(); ++it)
		bridge->SetField(*it);
}

bool ComponentScript::CallAwake()
{
	ScriptingBridgeComponentScript* bridge = (ScriptingBridgeComponentScript*)GetScriptingBridge();
	return bridge->CallAwake();
}

bool ComponentScript::CallStart()
{
	ScriptingBridgeComponentScript* bridge = (ScriptingBridgeComponentScript*)GetScriptingBridge();
	return bridge->CallStart();
}

void ComponentScript::CallUpdate()
{
	ScriptingBridgeComponentScript* bridge = (ScriptingBridgeComponentScript*)GetScriptingBridge();
	bridge->CallUpdate();
}

void ComponentScript::CallOnDestroy()
{
	ScriptingBridgeComponentScript* bridge = (ScriptingBridgeComponentScript*)GetScriptingBridge();
	bridge->CallOnDestroy();
}

void ComponentScript::DrawFieldValue(ComponentScriptField* field_value)
{
	std::string field_name = field_value->GetName();

	switch (field_value->GetType())
	{
	case ScriptFieldType::SCRIPT_FIELD_INT:
	{
		ComponentScriptFieldInt* field_obj = (ComponentScriptFieldInt*)field_value;

		int val = field_obj->GetValue();

		if (ImGui::DragInt(field_name.c_str(), &val))
			field_obj->SetValue(val);
		
		break;
	}

	case ScriptFieldType::SCRIPT_FIELD_FLOAT:
	{
		ComponentScriptFieldFloat* field_obj = (ComponentScriptFieldFloat*)field_value;

		float val = field_obj->GetValue();

		if (ImGui::DragFloat(field_name.c_str(), &val))
			field_obj->SetValue(val);

		break;
	}

	case ScriptFieldType::SCRIPT_FIELD_STRING:
	{
		ComponentScriptFieldString* field_obj = (ComponentScriptFieldString*)field_value;

		std::string val = field_obj->GetValue();

		char text[999];
		memset(text, 0, 999 * sizeof(char));
		TextCpy(text, val.c_str());

		if (ImGui::InputText(field_name.c_str(), text, 998))
			field_obj->SetValue(text);
		
		break;
	}

	case ScriptFieldType::SCRIPT_FIELD_BOOL:
	{
		ComponentScriptFieldBool* field_obj = (ComponentScriptFieldBool*)field_value;

		bool val = field_obj->GetValue();

		if (ImGui::Checkbox(field_name.c_str(), &val))
			field_obj->SetValue(val);

		break;
	}

	case ScriptFieldType::SCRIPT_FIELD_GAMEOBJECT:
	{
		ComponentScriptFieldGameObject* field_obj = (ComponentScriptFieldGameObject*)field_value;

		GameObject* val = field_obj->GetValue();

		if (go_selector_widget.Draw(field_name.c_str(), val))
			field_obj->SetValue(val);

		break;
	}

	case ScriptFieldType::SCRIPT_FIELD_RESOURCE_PREFAB:
	{
		ComponentScriptFieldResourcePrefab* field_obj = (ComponentScriptFieldResourcePrefab*)field_value;

		Resource* val = (Resource*)field_obj->GetValue();

		if(resource_selector_widget.Draw(field_name.c_str(), ResourceType::RESOURCE_TYPE_PREFAB, val))
			field_obj->SetValue((ResourcePrefab*)val);

		break;
	}

	case ScriptFieldType::SCRIPT_FIELD_RESOURCE_SCENE:
	{
		ComponentScriptFieldResourceScene* field_obj = (ComponentScriptFieldResourceScene*)field_value;

		Resource* val = (Resource*)field_obj->GetValue();

		if (resource_selector_widget.Draw(field_name.c_str(), ResourceType::RESOURCE_TYPE_SCENE, val))
			field_obj->SetValue((ResourceScene*)val);

		break;
	}

	}
}

void ComponentScript::RecalculateFieldsValues(const std::vector<ResourceScriptField>& fields)
{
	std::vector<ComponentScriptField*> fields_values_to_check = fields_values;

	std::vector<ComponentScriptField*> new_fields;

	for (std::vector<ResourceScriptField>::const_iterator fi = fields.begin(); fi != fields.end(); ++fi)
	{
		ComponentScriptField* field = nullptr;

		bool alredy_exists = false;

		for (std::vector<ComponentScriptField*>::iterator it = fields_values_to_check.begin(); it != fields_values_to_check.end(); ++it)
		{
			if ((*it)->GetType() == (*it)->GetType())
			{
				if ((*it)->GetName().compare((*fi).GetName()) == 0)
				{
					alredy_exists = true;

					new_fields.push_back((*it));

					field = (*it);
					fields_values_to_check.erase(it);
					break;
				}
			}
		}

		if (field == nullptr)
		{
			switch ((*fi).GetType())
			{
			case ScriptFieldType::SCRIPT_FIELD_INT:
			{
				field = new ComponentScriptFieldInt((*fi).GetName());
				break;
			}

			case ScriptFieldType::SCRIPT_FIELD_FLOAT:
			{
				field = new ComponentScriptFieldFloat((*fi).GetName());
				break;
			}

			case ScriptFieldType::SCRIPT_FIELD_STRING:
			{
				field = new ComponentScriptFieldString((*fi).GetName());
				break;
			}

			case ScriptFieldType::SCRIPT_FIELD_BOOL:
			{
				field = new ComponentScriptFieldBool((*fi).GetName());
				break;
			}

			case ScriptFieldType::SCRIPT_FIELD_GAMEOBJECT:
			{
				field = new ComponentScriptFieldGameObject((*fi).GetName());
				break;
			}

			case ScriptFieldType::SCRIPT_FIELD_RESOURCE_PREFAB:
			{
				field = new ComponentScriptFieldResourcePrefab((*fi).GetName());
				break;
			}

			case ScriptFieldType::SCRIPT_FIELD_RESOURCE_SCENE:
			{
				field = new ComponentScriptFieldResourceScene((*fi).GetName());
				break;
			}

			}
		}

		if (field != nullptr && !alredy_exists)
		{
			new_fields.push_back(field);
		}
	}

	fields_values.clear();

	fields_values = new_fields;

	RemoveFieldValues(fields_values_to_check);
}

void ComponentScript::RemoveFieldValues(const std::vector<ComponentScriptField*>& vals)
{
	for (std::vector<ComponentScriptField*>::const_iterator it = vals.begin(); it != vals.end(); ++it)
	{
		for (std::vector<ComponentScriptField*>::iterator fi = fields_values.begin(); fi != fields_values.end(); ++fi)
		{
			if ((*it) == (*fi))
			{
				RELEASE(*fi);
				fields_values.erase(fi);
				break;
			}
		}
	}
}

void ComponentScript::RemoveAllFieldValues()
{
	for (std::vector<ComponentScriptField*>::iterator fi = fields_values.begin(); fi != fields_values.end(); ++fi)
	{		
		RELEASE(*fi);
	}

	fields_values.clear();
}

void ComponentScript::CheckFieldGameObjectDestroyed(GameObject * go)
{
	if (go != nullptr)
	{
		for (std::vector<ComponentScriptField*>::iterator fi = fields_values.begin(); fi != fields_values.end();)
		{
			bool to_delete = false;

			if ((*fi)->GetType() == ScriptFieldType::SCRIPT_FIELD_GAMEOBJECT)
			{
				ComponentScriptFieldGameObject* field_go = (ComponentScriptFieldGameObject*)(*fi);

				if (field_go->GetValue() == go)
				{
					to_delete = true;
				}
			}

			if (to_delete)
			{
				RELEASE(*fi);
				fi = fields_values.erase(fi);
			}
			else
				++fi;
		}
	}
}

void ComponentScript::CheckFieldResourceDestroyed(Resource * res)
{
	if (res != nullptr)
	{
		for (std::vector<ComponentScriptField*>::iterator fi = fields_values.begin(); fi != fields_values.end();)
		{
			bool to_delete = false;

			switch ((*fi)->GetType())
			{
			case ScriptFieldType::SCRIPT_FIELD_RESOURCE_PREFAB:
			{
				ComponentScriptFieldResourcePrefab* pref = (ComponentScriptFieldResourcePrefab*)(*fi);

				if (res == (Resource*)pref->GetValue())
				{
					to_delete = true;
				}

				break;
			}

			case ScriptFieldType::SCRIPT_FIELD_RESOURCE_SCENE:
			{
				ComponentScriptFieldResourceScene* scen = (ComponentScriptFieldResourceScene*)(*fi);

				if (res == (Resource*)scen->GetValue())
				{
					to_delete = true;
				}

				break;
			}

			default:
				break;
			}

			if (to_delete)
			{
				RELEASE(*fi);
				fi = fields_values.erase(fi);
			}
			else
				++fi;
		}
	}
}
