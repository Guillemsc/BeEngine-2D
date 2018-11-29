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

	Resource* res = resource_script;
	if (App->resource->EditorResourceSelector(ResourceType::RESOURCE_TYPE_SCRIPT, res, resource_filter))
	{
		resource_script = (ResourceScript*)res;
	}
}

void ComponentScript::Start()
{
	
}

void ComponentScript::CleanUp()
{
	
}

void ComponentScript::OnEvent(Event * ev)
{
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
