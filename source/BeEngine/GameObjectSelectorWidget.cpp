#include "GameObjectSelectorWidget.h"
#include "GameObject.h"
#include "ModuleGameObject.h"
#include "App.h"
#include "Functions.h"

GameObjectSelectorWidget::GameObjectSelectorWidget()
{
}

bool GameObjectSelectorWidget::Draw(const std::string & selector_name, GameObject *& go)
{
	bool ret = false;

	std::string go_name = "";

	if (go != nullptr)
	{
		go_name = go->GetName();
	}
	else
	{
		go_name = "None (GameObject)";
	}

	std::string name = selector_name + "    ";

	ImGui::PushID(name.c_str());

	ImGui::Text(name.c_str());

	ImGui::SameLine();

	ImGui::Text(go_name.c_str());

	ImGui::SameLine();

	bool open_popup_search_resource = false;

	if (ImGui::SmallButton("+"))
	{
		open_popup_search_resource = true;
	}

	if (open_popup_search_resource)
	{
		ImGui::OpenPopup("SearchGosPopup");
	}

	if (ImGui::BeginPopup("SearchGosPopup"))
	{
		std::vector<GameObject*> gos = App->gameobject->GetGameObjects();

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

		for (std::vector<GameObject*>::iterator it = gos.begin(); it != gos.end();)
		{
			bool contains = true;

			if (!text_empty)
			{
				std::string name = (*it)->GetName();

				contains = StringContainsFormated(name, filter_text);
			}

			if ((*it) == go)
				contains = false;

			if (!contains)
				it = gos.erase(it);
			else
				++it;
		}


		if (ImGui::SmallButton("None"))
		{
			go = nullptr;
			ret = true;
			ImGui::CloseCurrentPopup();
		}

		if (gos.size() > 0)
			ImGui::Separator();

		for (std::vector<GameObject*>::iterator it = gos.begin(); it != gos.end(); ++it)
		{
			std::string name = (*it)->GetName();

			if (ImGui::SmallButton(name.c_str()))
			{
				go = (*it);
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
