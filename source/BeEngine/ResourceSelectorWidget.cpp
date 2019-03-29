#include "ResourceSelectorWidget.h"
#include "Resource.h"
#include "ModuleResource.h"
#include "App.h"
#include "Functions.h"
#include "GameObjectSelectorWidget.h"

ResourceSelectorWidget::ResourceSelectorWidget()
{
}

bool ResourceSelectorWidget::Draw(const std::string & selector_name, const ResourceType type, Resource *& res)
{
	bool ret = false;

	std::string resource_name = "";

	if (res != nullptr)
	{
		resource_name = res->GetDecomposedAssetFilepath().file_name + "." + res->GetDecomposedAssetFilepath().file_extension_lower_case.c_str();
	}
	else
	{
		std::string resource_type_name = App->resource->GetResourceNameFromResourceType(type);
		resource_name = "None (" + resource_type_name + ")";
	}

	std::string name = selector_name + "    ";

	ImGui::PushID(name.c_str());

	ImGui::Text(name.c_str());

	ImGui::SameLine();

	ImGui::Text(resource_name.c_str());

	ImGui::SameLine();

	bool open_popup_search_resource = false;

	if (ImGui::SmallButton("+"))
	{
		open_popup_search_resource = true;
	}

	if (open_popup_search_resource)
	{
		ImGui::OpenPopup("SearchResourcePopup");
	}

	if (ImGui::BeginPopup("SearchResourcePopup"))
	{
		std::vector<Resource*> resources = App->resource->GetResourcesFromResourceType(type);

		char filter[50];
		memset(filter, 0, sizeof(char) * 50);

		int size = 50;

		if (filter_text.size() + 1 < 50)
			size = filter_text.size() + 1;

		strcpy_s(filter, sizeof(char)* size, filter_text.c_str());

		if (ImGui::InputText("Filter Search", filter, sizeof(char) * 50, ImGuiInputTextFlags_AutoSelectAll))
		{
			filter_text = filter;
		}

		bool text_empty = filter_text.compare("") == 0;

		for (std::vector<Resource*>::iterator it = resources.begin(); it != resources.end();)
		{
			bool contains = true;


			if (!text_empty)
			{
				std::string name = (*it)->GetDecomposedAssetFilepath().file_name + "." + (*it)->GetDecomposedAssetFilepath().file_extension_lower_case.c_str();

				contains = StringContainsFormated(name, filter_text);
			}

			if ((*it) == res)
				contains = false;

			if (!contains)
				it = resources.erase(it);
			else
				++it;
		}


		if (ImGui::SmallButton("None"))
		{
			res = nullptr;
			ret = true;
			ImGui::CloseCurrentPopup();
		}

		if (resources.size() > 0)
			ImGui::Separator();

		for (std::vector<Resource*>::iterator it = resources.begin(); it != resources.end(); ++it)
		{
			std::string name = (*it)->GetDecomposedAssetFilepath().file_name + "." + (*it)->GetDecomposedAssetFilepath().file_extension_lower_case.c_str();

			if (ImGui::SmallButton(name.c_str()))
			{
				res = (*it);
				ret = true;
				ImGui::CloseCurrentPopup();
				break;
			}
		}

		ImGui::EndPopup();
	}
	else
	{
		filter_text = "";
	}

	ImGui::PopID();

	return ret;
}
