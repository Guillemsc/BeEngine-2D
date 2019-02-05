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

ComponentScript::ComponentScript() : GameObjectComponent("Script", ComponentType::COMPONENT_TYPE_SCRIPT, ComponentGroup::SCRIPTING)
{
}

ComponentScript::~ComponentScript()
{
}

void ComponentScript::EditorDraw()
{
	ImGui::Text("Script:   ");

	ImGui::SameLine();

	bool open_error_script = false;

	Resource* res = resource_script;
	if (App->resource->EditorResourceSelector(ResourceType::RESOURCE_TYPE_SCRIPT, res, resource_filter))
	{
		ResourceScript* resource = (ResourceScript*)res;
		if (resource != nullptr && !resource->GetInheritsFromBeengineReference())
		{
			open_error_script = true;
		}
		else
		{
			ClearFieldsValues();

			resource_script = (ResourceScript*)res;

			if (resource_script != nullptr)
			{
				RecalculateFieldsValues(resource_script->GetFields());
			}
		}
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
		ImGui::Text("needs to derive from BeEngineReference.");

		ImGui::Spacing();
		if (ImGui::Button("Ok", ImVec2(230, 0)))
		{
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (resource_script != nullptr)
	{
		for (std::vector<ResourceScriptFieldValue>::iterator it = fields_values.begin(); it != fields_values.end(); ++it)
		{
			DrawFieldValue((*it));
		}
	}
}

void ComponentScript::Start()
{
	App->event->Suscribe(std::bind(&ComponentScript::OnEvent, this, std::placeholders::_1), EventType::RESOURCE_DESTROYED);
	App->event->Suscribe(std::bind(&ComponentScript::OnEvent, this, std::placeholders::_1), EventType::RESOURCE_SCRIPTS_FIELDS_CHANGED);
}

void ComponentScript::CleanUp()
{
	App->event->UnSuscribe(std::bind(&ComponentScript::OnEvent, this, std::placeholders::_1), EventType::RESOURCE_DESTROYED);
	App->event->UnSuscribe(std::bind(&ComponentScript::OnEvent, this, std::placeholders::_1), EventType::RESOURCE_SCRIPTS_FIELDS_CHANGED);
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

			ClearFieldsValues();
		}
		break;
	}
	case EventType::RESOURCE_SCRIPTS_FIELDS_CHANGED:
	{
		if (resource_script != nullptr)
		{
			if (!resource_script->GetInheritsFromBeengineReference())
			{
				resource_script = nullptr;
				break;
			}
			else
			{
				RecalculateFieldsValues(resource_script->GetFields());
			}
		}
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

void ComponentScript::CreateScriptInstance()
{
	if (resource_script != nullptr)
	{
		if (resource_script->GetInheritsFromBeengineReference())
		{
			if (App->scripting->user_code_assembly != nullptr && App->scripting->scripting_assembly != nullptr)
			{
				if (App->scripting->user_code_assembly->GetAssemblyLoaded() && App->scripting->scripting_assembly->GetAssemblyLoaded())
				{
					DecomposedFilePath dfp = resource_script->GetDecomposedAssetFilepath();

					ScriptingClass script_class = resource_script->GetScriptingClass();

					script_instance = script_class.CreateInstance();

					if (script_instance != nullptr)
					{
						owner->transform->SetPosition(float2(3, 0));
						 
						MonoArray* pointer_arr = App->scripting->BoxPointer(owner);

						void* args[1] = { pointer_arr };

						ScriptingClass be_engine_script_class;
						if (script_instance->GetClass().GetParentClass(be_engine_script_class))
						{
							MonoObject* ret_obj = nullptr;
							if (script_instance->InvokeMonoMethodOnParentClass(be_engine_script_class, "InitReference", args, 1, ret_obj))
							{
								GameObject* new_pointer = (GameObject*)App->scripting->UnboxPointer((MonoArray*)ret_obj);

								if (new_pointer == owner)
								{
									int i = 0;
								}
							}
						}
					}
				}
			}
		}
	}
}

void ComponentScript::DestroyScriptInstance()
{
	if (resource_script != nullptr)
	{
		if (script_instance != nullptr)
		{
			script_instance->CleanUp();
			RELEASE(script_instance);
		}
	}
}

void ComponentScript::CallAwake()
{
	if (resource_script != nullptr)
	{
		if (script_instance != nullptr)
		{
			MonoObject* ret_obj = nullptr;
			if (script_instance->InvokeMonoMethod("Awake", nullptr, 1, ret_obj))
			{

			}
		}
	}
}

void ComponentScript::CallStart()
{
	if (resource_script != nullptr)
	{
		if (script_instance != nullptr)
		{
			MonoObject* ret_obj = nullptr;
			if (script_instance->InvokeMonoMethod("Start", nullptr, 1, ret_obj))
			{

			}
		}
	}
}

void ComponentScript::CallUpdate()
{
	if (resource_script != nullptr)
	{
		if (script_instance != nullptr)
		{
			MonoObject* ret_obj = nullptr;
			if (script_instance->InvokeMonoMethod("Update", nullptr, 1, ret_obj))
			{

			}
		}
	}
}

void ComponentScript::CallOnDestroy()
{
	if (resource_script != nullptr)
	{
		if (script_instance != nullptr)
		{
			MonoObject* ret_obj = nullptr;
			if (script_instance->InvokeMonoMethod("OnDestroy", nullptr, 1, ret_obj))
			{

			}
		}
	}
}

void ComponentScript::DrawFieldValue(ResourceScriptFieldValue & field_value)
{
	std::string field_name = field_value.GetFieldName();

	switch (field_value.GetFieldType())
	{
	case ResourceScriptFieldType::SCRIPT_FIELD_BOOL:
	{
		if (ImGui::Checkbox(field_name.c_str(), &field_value.bool_field))
		{

		}
		break;
	}
	case ResourceScriptFieldType::SCRIPT_FIELD_INT:
	{
		if (ImGui::DragInt(field_name.c_str(), &field_value.int_field))
		{

		}
		break;
	}
	case ResourceScriptFieldType::SCRIPT_FIELD_FLOAT:
	{
		if (ImGui::DragFloat(field_name.c_str(), &field_value.float_field))
		{

		}
		break;
	}
	case ResourceScriptFieldType::SCRIPT_FIELD_STRING:
	{
		char text[999];
		memset(text, 0, 999 * sizeof(char));
		TextCpy(text, field_value.string_field.c_str());

		if (ImGui::InputText(field_name.c_str(), text, 999))
		{
			field_value.string_field = text;
		}
		break;
	}
	default:
		break;
	}
}

void ComponentScript::ClearFieldsValues()
{
	fields_values.clear();
}

void ComponentScript::RecalculateFieldsValues(std::vector<ResourceScriptField> fields)
{
	std::vector<ResourceScriptFieldValue> fields_values_to_check = fields_values;

	fields_values.clear();

	for (std::vector<ResourceScriptField>::const_iterator fi = fields.begin(); fi != fields.end(); ++fi)
	{
		bool found = false;

		for (std::vector<ResourceScriptFieldValue>::iterator it = fields_values_to_check.begin(); it != fields_values_to_check.end(); ++it)
		{	
			if ((*it).GetFieldName().compare((*fi).field_name) == 0)
			{
				fields_values.push_back((*it));
				fields_values_to_check.erase(it);
				found = true;
				break;
			}
		}

		if (!found)
		{
			ResourceScriptFieldValue nfv((*fi));
			fields_values.push_back(nfv);
		}
	}

}
