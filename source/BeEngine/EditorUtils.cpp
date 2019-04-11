#include "EditorUtils.h"
#include "imgui.h"
#include "GameObject.h"
#include "App.h"
#include "ModuleGameObject.h"
#include "ModuleFileSystem.h"
#include "ModuleAssets.h"

bool EditorUtils::NewGameObjectButton(GameObject* parent)
{
	bool ret = false;

	if (ImGui::Button("New GameObject"))
	{
		GameObject* created_go = App->gameobject->CreateGameObject();

		if (parent != nullptr)
			created_go->SetParent(parent);

		ret = true;
	}

	return ret;
}

bool EditorUtils::DuplicateGameObjectButton(GameObject * to_duplicate)
{
	bool ret = false;

	if (ImGui::Button("Duplicate"))
	{
		if (to_duplicate != nullptr)
		{
			std::vector<GameObject*> to_duplicate_list;
			to_duplicate_list.push_back(to_duplicate);

			std::vector<GameObject*> created_gos;

			GameObjectAbstraction abs;
			abs.Abstract(to_duplicate_list);

			created_gos = abs.DeAbstract();

			if (created_gos.size() > 0)
			{
				GameObject* created_go = created_gos[0];

				created_go->SetParent(to_duplicate->GetParent());

				created_go->SetName((created_go->GetName() + "_Copy").c_str());
			}

			ret = true;
		}
	}

	return ret;
}

void EditorUtils::ImportAssetButton()
{
	bool canceled = false;

	const char* filter[1] = { "" };
	std::string file = App->file_system->SelectFileDilog(canceled, filter);

	if (!canceled)
	{
		App->assets->LoadFileToEngine(file.c_str());
	}
}
