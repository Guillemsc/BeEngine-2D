#include "ResourcesWindow.h"
#include "App.h"
#include "ModuleResource.h"

ResourcesWindow::ResourcesWindow()
{
}

ResourcesWindow::~ResourcesWindow()
{
}

void ResourcesWindow::Start()
{
}

void ResourcesWindow::CleanUp()
{
}

void ResourcesWindow::DrawEditor()
{
	std::map<ResourceType, std::vector<Resource*>> resources = App->resource->GetAllResources();

	for (std::map<ResourceType, std::vector<Resource*>>::iterator it = resources.begin(); it != resources.end(); ++it)
	{
		ResourceType curr_type = (*it).first;
		std::string resource_name = App->resource->GetResourceNameFromResourceType(curr_type);
		
		if (ImGui::CollapsingHeader(resource_name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			//ImGui::SameLine();

			//std::string count_text = "Count: " + std::to_string((*it).second.size());
			//ImGui::Text(count_text.c_str());

			for (std::vector<Resource*>::iterator t = (*it).second.begin(); t != (*it).second.end(); ++t)
			{
				ImGui::Text((*t)->GetUID().c_str());
			}

			ImGui::Separator();
		}
	}
}

ImGuiWindowFlags ResourcesWindow::GetWindowFlags()
{
	return ImGuiWindowFlags();
}
