#include "InspectorWindow.h"
#include "App.h"
#include "ModuleGameObject.h"
#include "GameObjectComponent.h"
#include "imgui.h"
#include "ModuleEvent.h"
#include "Event.h"
#include "Resource.h"
#include "ModuleResource.h"
#include "Functions.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

InspectorWindow::InspectorWindow()
{
}

InspectorWindow::~InspectorWindow()
{
}

void InspectorWindow::Start()
{
	App->event->Suscribe(std::bind(&InspectorWindow::OnEvent, this, std::placeholders::_1), EventType::GAME_OBJECT_DESTROYED);
	App->event->Suscribe(std::bind(&InspectorWindow::OnEvent, this, std::placeholders::_1), EventType::GAME_OBJECT_CREATED);
	App->event->Suscribe(std::bind(&InspectorWindow::OnEvent, this, std::placeholders::_1), EventType::RESOURCE_DESTROYED);
}

void InspectorWindow::CleanUp()
{
	App->event->UnSuscribe(std::bind(&InspectorWindow::OnEvent, this, std::placeholders::_1), EventType::GAME_OBJECT_DESTROYED);
	App->event->UnSuscribe(std::bind(&InspectorWindow::OnEvent, this, std::placeholders::_1), EventType::GAME_OBJECT_CREATED);
	App->event->UnSuscribe(std::bind(&InspectorWindow::OnEvent, this, std::placeholders::_1), EventType::RESOURCE_DESTROYED);
}

void InspectorWindow::DrawEditor()
{
	switch (showing)
	{
	case SHOWING_GAMEOBJECTS:
		DrawGameObjects();
		break;
	case SHOWING_RESOURCES:
		DrawResources();
		break;
	default:
		break;
	}
}

ImGuiWindowFlags InspectorWindow::GetWindowFlags()
{
	return ImGuiWindowFlags();
}

void InspectorWindow::SetShowingGos(const std::vector<GameObject*>& gos)
{
	showing_gos.clear();

	showing_gos = gos;

	showing = InspectorWindowShowing::SHOWING_GAMEOBJECTS;
}

void InspectorWindow::SetShowingGos(GameObject * go)
{
	if (go != nullptr)
	{
		std::vector<GameObject*> gos;
		gos.push_back(go);

		SetShowingGos(gos);
	}
}

void InspectorWindow::RemoveFromShowingGos(GameObject * go)
{
	if (go != nullptr)
	{
		for (std::vector<GameObject*>::iterator it = showing_gos.begin(); it != showing_gos.end(); ++it)
		{
			if ((*it) == go)
			{
				showing_gos.erase(it);
				break;
			}
		}
	}
}

void InspectorWindow::SetShowingResources(const std::vector<Resource*>& res)
{
	showing_resources.clear();

	showing_resources = res;

	showing = InspectorWindowShowing::SHOWING_RESOURCES;
}

void InspectorWindow::SetShowingResource(Resource * res)
{
	if (res != nullptr)
	{
		std::vector<Resource*> reso;
		reso.push_back(res);

		SetShowingResources(reso);
	}
}

void InspectorWindow::RemoveFromShowingResources(Resource * res)
{
	if (res != nullptr)
	{
		for (std::vector<Resource*>::iterator it = showing_resources.begin(); it != showing_resources.end(); ++it)
		{
			if ((*it) == res)
			{
				showing_resources.erase(it);
				break;
			}
		}
	}
}

void InspectorWindow::OnEvent(Event * ev)
{
	if (ev->GetType() == EventType::GAME_OBJECT_DESTROYED)
	{
		EventGameObjectDestroyed* evd = (EventGameObjectDestroyed*)ev;

		RemoveFromShowingGos(evd->GetGameObject());
	}
	else if (ev->GetType() == EventType::GAME_OBJECT_CREATED)
	{
		EventGameObjectCreated* evc = (EventGameObjectCreated*)ev;

		SetShowingGos(evc->GetGameObject());
	}
	else if (ev->GetType() == EventType::RESOURCE_DESTROYED)
	{
		EventResourceDestroyed* evd = (EventResourceDestroyed*)ev;

		RemoveFromShowingResources(evd->GetResource());
	}
}

void InspectorWindow::DrawGameObjects()
{
	float2 win_size = GetWindowSize();

	if (showing_gos.size() == 1)
	{
		GameObject* selected_go = *showing_gos.begin();

		std::string name = selected_go->GetName();
		if (name.size() > 99)
			name = name.substr(0, 98);

		char name_arr[99];
		strcpy_s(name_arr, 99, name.c_str());
		if (ImGui::InputText("", name_arr, 50, ImGuiInputTextFlags_AutoSelectAll))
			selected_go->SetName(name_arr);

		ImGui::SameLine();

		bool active = selected_go->GetActive();

		if (ImGui::Checkbox("Active", &active))
			selected_go->SetActive(active);

		ImGui::Separator();

	}

	int count = 0;
	for (std::vector<GameObject*>::iterator it = showing_gos.begin(); it != showing_gos.end(); ++it)
	{
		GameObject* curr_game_object = (*it);

		std::vector<GameObjectComponent*> components = curr_game_object->GetComponents();

		ImGui::SameLine();

		if (count == 0)
		{
			for (std::vector<GameObjectComponent*>::iterator com = components.begin(); com != components.end(); ++com)
			{
				ImGui::Spacing();
				ImGui::Spacing();
				ImGui::Spacing();

				GameObjectComponent* curr_component = (*com);

				ImGui::PushID(curr_component->GetUID().c_str());

				if (ImGui::CollapsingHeader(curr_component->GetName().c_str(), ImGuiTreeNodeFlags_DefaultOpen))
				{
					if (curr_component->GetCanDestroy())
					{
						if (ImGui::SmallButton("Delete"))
						{
							curr_game_object->DestroyComponent(curr_component);
						}

						ImGui::SameLine();
					}

					if (ImGui::SmallButton("Move Up"))
					{

					}

					ImGui::SameLine();


					if (ImGui::SmallButton("Move Down"))
					{

					}

					ImGui::Separator();

					ImGui::Spacing();
					ImGui::Spacing();
					ImGui::Spacing();

					ImGui::Indent(15);

					curr_component->EditorDraw();

					ImGui::Indent(-15);
				}

				ImGui::Separator();

				ImGui::PopID();
			}
		}

		++count;
	}

	bool open_components_popup = false;

	if (showing_gos.size() > 0)
	{
		ImGui::Spacing();

		if (ImGui::Button("Add Component", ImVec2(win_size.x - 12, 30)))
		{
			open_components_popup = true;
		}
	}

	if (open_components_popup)
	{
		ImGui::OpenPopup("CreateComponentPopup");
	}

	DrawComponentsPopup(showing_gos);
}

void InspectorWindow::DrawResources()
{
	if (showing_resources.size() == 1)
	{
		Resource* res = showing_resources[0];

		std::string type = App->resource->GetResourceNameFromResourceType(res->GetType());

		type = ToUpperCase(type);

		ImGui::Text(type.c_str());

		ImGui::SameLine();

		ImGui::Text(res->GetDecomposedAssetFilepath().file_name.c_str());

		ImGui::Separator();

		res->DrawEditorInspector();
	}
}

void InspectorWindow::DrawComponentsPopup(const std::vector<GameObject*>& selected_gos)
{
	if (ImGui::BeginPopup("CreateComponentPopup"))
	{
		ImGui::Text("Components:");

		ImGui::Separator();

		std::vector<GameObjectComponentData> components_data = App->gameobject->GetComponentsData();

		for (std::vector<GameObjectComponentData>::iterator it = components_data.begin(); it != components_data.end(); ++it)
		{
			if(ImGui::Button((*it).GetName().c_str()))
			{
				for (std::vector<GameObject*>::const_iterator go = selected_gos.begin(); go != selected_gos.end(); ++go)
				{
					(*go)->CreateComponent((*it).GetType());
				}

				ImGui::CloseCurrentPopup();
				break;
			}
		}

		ImGui::EndPopup();
	}
}
