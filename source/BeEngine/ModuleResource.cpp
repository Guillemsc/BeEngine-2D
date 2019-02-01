#include "ModuleResource.h"
#include "Functions.h"
#include "App.h"
#include "ModuleFileSystem.h"
#include "ModuleEvent.h"
#include "ModuleProject.h"
#include "ModuleEditor.h"
#include "ModuleScripting.h" 
#include "ScriptingObjectFileWatcher.h"
#include "ModuleJson.h"
#include "ModuleInput.h"
#include "ResourceTexture.h"
#include "ResourceScript.h"
#include "ScriptingObjectCompiler.h"
#include "ModuleInput.h"
#include "ModuleScripting.h"
#include "ScriptingObjectSolutionManager.h"
#include "Event.h"
#include "ModuleAssets.h"

#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")

ModuleResource::ModuleResource()
{

}

ModuleResource::~ModuleResource()
{
	
}

bool ModuleResource::Awake()
{
	bool ret = true;

	AddResourceName(ResourceType::RESOURCE_TYPE_TEXTURE, "texture");
	AddResourceName(ResourceType::RESOURCE_TYPE_SCRIPT, "script");

	AddAssetExtension(ResourceType::RESOURCE_TYPE_TEXTURE, "png");
	AddAssetExtension(ResourceType::RESOURCE_TYPE_TEXTURE, "jpg");

	AddAssetExtension(ResourceType::RESOURCE_TYPE_SCRIPT, "cs");

	return ret;
}

bool ModuleResource::Start()
{
	bool ret = true;

	return ret;
}

bool ModuleResource::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool ModuleResource::Update()
{
	bool ret = true;

	return ret;
}

bool ModuleResource::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool ModuleResource::CleanUp()
{
	bool ret = true;

	DestroyAllResources();

	return ret;
}

std::string ModuleResource::GetNewUID()
{	
	return GetUIDRandomHexadecimal();
}

std::string ModuleResource::GetResourceNameFromResourceType(const ResourceType & type)
{
	return resource_names[type];
}

std::string ModuleResource::GetLibraryPathFromResourceType(const ResourceType & type)
{
	std::string ret = "";

	ret += App->assets->GetLibraryPath();

	ret += resource_names[type] + "\\";

	return ret;
}

Resource* ModuleResource::CreateResource(const ResourceType type)
{
	Resource* ret = nullptr;

	switch (type)
	{
	case ResourceType::RESOURCE_TYPE_TEXTURE:

		ret = new ResourceTexture();
		
		break;

	case ResourceType::RESOURCE_TYPE_SCRIPT:

		ret = new ResourceScript();

		break;
	default:
		break;
	}

	if (ret != nullptr)
	{
		std::map<ResourceType, std::vector<Resource*>>::iterator it = resources.find(type);

		if (it != resources.end())
		{
			it->second.push_back(ret);
		}
		else
		{
			std::vector<Resource*> new_vec;
			new_vec.push_back(ret);

			resources[type] = new_vec;
		}
	}

	return ret;
}

void ModuleResource::DestroyResource(Resource * res)
{
	for (std::map<ResourceType, std::vector<Resource*>>::iterator it = resources.begin(); it != resources.end(); ++it)
	{
		bool found = false;
		for (std::vector<Resource*>::iterator t = (*it).second.begin(); t != (*it).second.end(); ++t)
		{
			if ((*t) == res)
			{
				EventResourceDestroyed* event_res_destroyed = new EventResourceDestroyed(res);
				App->event->SendEvent(event_res_destroyed);

				res->CleanUp();

				RELEASE(res);
				(*it).second.erase(t);

				found = true;
				break;
			}
		}

		if (found)
			break;
	}
}

Resource* ModuleResource::GetResourceFromAssetFile(const char* filepath)
{
	Resource* ret = nullptr;

	for (std::map<ResourceType, std::vector<Resource*>>::iterator it = resources.begin(); it != resources.end(); ++it)
	{
		bool found = false;
		for (std::vector<Resource*>::iterator t = (*it).second.begin(); t != (*it).second.end(); ++t)
		{
			if ((*t)->asset_filepath.compare(filepath) == 0)
			{
				ret = (*t);
				found = true;
				break;
			}
		}

		if (found)
			break;
	}

	return ret;
}

std::vector<Resource*> ModuleResource::GetResourcesFromResourceType(const ResourceType type)
{
	std::vector<Resource*> ret;

	for (std::map<ResourceType, std::vector<Resource*>>::iterator it = resources.begin(); it != resources.end(); ++it)
	{
		if ((*it).first == type)
		{
			ret = (*it).second;
			break;
		}
	}

	return ret;
}

std::map<ResourceType, std::vector<Resource*>> ModuleResource::GetAllResources() const
{
	return resources;
}

std::map<ResourceType, std::string> ModuleResource::GetAllResourcesNames() const
{
	return resource_names;
}

std::vector<Resource*> ModuleResource::GetResourcesOnAssetsPath(const std::string& path)
{
	std::vector<Resource*> ret;

	for (std::map<ResourceType, std::vector<Resource*>>::iterator it = resources.begin(); it != resources.end(); ++it)
	{
		for (std::vector<Resource*>::iterator t = (*it).second.begin(); t != (*it).second.end(); ++t)
		{
			if ((*t)->d_asset_filepath.path.compare(path) == 0)
			{
				ret.push_back(*t);
			}
		}
	}

	return ret;
}

bool ModuleResource::EditorResourceSelector(const ResourceType type, Resource *& res, std::string& filter_text)
{
	bool ret = false;

	std::string resource_name = "";

	if (res != nullptr)
	{
		resource_name = res->GetDecomposedAssetFilepath().file_name + "." + res->GetDecomposedAssetFilepath().file_extension_lower_case.c_str();
	}
	else
	{
		std::string resource_type_name = GetResourceNameFromResourceType(type);
		resource_name = "None (" + resource_type_name + ")";
	}

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
		std::vector<Resource*> resources = GetResourcesFromResourceType(type);

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

		if(resources.size() > 0)
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

	return ret;
}

void ModuleResource::AddAssetExtension(const ResourceType & type, const char * extension)
{
	std::map<ResourceType, std::vector<std::string>>::iterator it = asset_extensions.find(type);

	if (it != asset_extensions.end())
	{
		it->second.push_back(extension);
	}
	else
	{
		std::vector<std::string> new_vec;
		new_vec.push_back(extension);

		asset_extensions[type] = new_vec;
	}
}

void ModuleResource::AddLibraryExtension(const ResourceType & type, const char * extension)
{
	std::map<ResourceType, std::vector<std::string>>::iterator it = library_extensions.find(type);

	if (it != library_extensions.end())
	{
		it->second.push_back(extension);
	}
	else
	{
		std::vector<std::string> new_vec;
		new_vec.push_back(extension);

		library_extensions[type] = new_vec;
	}
}

ResourceType ModuleResource::GetResourceTypeFromAssetExtension(const char * extension)
{
	ResourceType ret = ResourceType::RESOURCE_TYPE_UNKWNOWN;

	for (std::map<ResourceType, std::vector<std::string>>::iterator it = asset_extensions.begin(); it != asset_extensions.end(); ++it)
	{
		ResourceType curr_type = (*it).first;

		std::vector<std::string> extensions = (*it).second;

		for (std::vector<std::string>::iterator ex = extensions.begin(); ex != extensions.end(); ++ex)
		{
			if ((*ex).compare(extension) == 0)
			{
				ret = curr_type;
				break;
			}
		}
	}

	return ret;
}

ResourceType ModuleResource::GetResourceTypeFromLibraryExtension(const char * extension)
{
	ResourceType ret = ResourceType::RESOURCE_TYPE_UNKWNOWN;

	for (std::map<ResourceType, std::vector<std::string>>::iterator it = library_extensions.begin(); it != library_extensions.end(); ++it)
	{
		ResourceType curr_type = (*it).first;

		std::vector<std::string> extensions = (*it).second;

		for (std::vector<std::string>::iterator ex = extensions.begin(); ex != extensions.end(); ++ex)
		{
			if ((*ex).compare(extension) == 0)
			{
				ret = curr_type;
				break;
			}
		}
	}

	return ret;
}

void ModuleResource::AddResourceName(const ResourceType & type, const char * name)
{
	resource_names[type] = name;
}

void ModuleResource::DestroyAllResources()
{
	for (std::map<ResourceType, std::vector<Resource*>>::iterator it = resources.begin(); it != resources.end(); ++it)
	{
		for (std::vector<Resource*>::iterator t = (*it).second.begin(); t != (*it).second.end(); ++t)
		{			
			EventResourceDestroyed* event_res_destroyed = new EventResourceDestroyed(*t);
			App->event->SendEvent(event_res_destroyed);

			(*t)->CleanUp();

			RELEASE(*t);
		}

		(*it).second.clear();
	}

	resources.clear();
}
