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
	std::vector<Resource*> resources = App->resource->GetAllResources();

	for (std::vector<Resource*>::iterator it = resources.begin(); it != resources.end(); ++it)
	{
		Resource* curr_resource = (*it);

		ImGui::Text(curr_resource->GetUID().c_str());
	}
}

ImGuiWindowFlags ResourcesWindow::GetWindowFlags()
{
	return ImGuiWindowFlags();
}
