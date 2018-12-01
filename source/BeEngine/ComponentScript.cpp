#include "ComponentScript.h"
#include "imgui.h"
#include "App.h"
#include "ModuleResource.h"
#include "ResourceScript.h"
#include "ModuleEvent.h"
#include "Event.h"

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
			resource_script = (ResourceScript*)res;
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
		std::vector<ResourceScriptField> script_fields = resource_script->GetFields();

		for (std::vector<ResourceScriptField>::iterator it = script_fields.begin(); it != script_fields.end(); ++it)
		{
			ImGui::Text((*it).field_name.c_str());
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
			resource_script = nullptr;
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
