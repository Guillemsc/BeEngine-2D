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
	//std::map<ResourceType, ResourceLoader*> loaders = App->resource->GetLoaders();

	//for (std::map<ResourceType, ResourceLoader*>::iterator it = loaders.begin(); it != loaders.end(); ++it)
	//{
	//	ResourceLoader* curr_loader = (*it).second;

	//	if (ImGui::CollapsingHeader(curr_loader->GetResourcesToLoadName().c_str()))
	//	{
	//		std::map<std::string, Resource*> resources = curr_loader->GetResources();

	//		std::string counter = "Count: " + std::to_string(resources.size());

	//		ImGui::Text(counter.c_str());

	//		ImGui::Separator();

	//		for (std::map<std::string, Resource*>::iterator re = resources.begin(); re != resources.end(); ++re)
	//		{
	//			Resource* curr_resource = (*re).second;

	//			ImGui::Text(curr_resource->GetUID().c_str());
	//		}
	//	}
	//}
}

ImGuiWindowFlags ResourcesWindow::GetWindowFlags()
{
	return ImGuiWindowFlags();
}
