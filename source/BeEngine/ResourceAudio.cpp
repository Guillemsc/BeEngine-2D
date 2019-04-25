#include "ResourceAudio.h"
#include "App.h"
#include "ModuleResource.h"
#include "ModuleAudio.h"
#include "ScriptingBridgeResourceAudio.h"
#include "imgui.h"

ResourceAudio::ResourceAudio()
	: Resource(new ScriptingBridgeResourceAudio(this), ResourceType::RESOURCE_TYPE_AUDIO)
{
}

void ResourceAudio::CleanUp()
{
}

bool ResourceAudio::ExistsOnLibrary(std::string uid, std::string & library_filepath)
{
	bool ret = false;

	std::string library_path = App->resource->GetLibraryPathFromResourceType(GetType());

	std::string path = library_path + uid + "." + d_asset_filepath.file_extension;

	if (App->file_system->FileExists(path.c_str()))
	{
		library_filepath = path;
		ret = true;
	}

	return ret;
}

void ResourceAudio::ExportToLibrary(std::string uid)
{
	std::string library_path = App->resource->GetLibraryPathFromResourceType(GetType());

	std::string resultant_filepath;
	if (App->file_system->FileCopyPaste(GetAssetFilepath(), library_path, true, resultant_filepath))
	{
		std::string new_filepath = library_path + uid + "." + d_asset_filepath.file_extension;

		if (App->file_system->FileExists(new_filepath))
			App->file_system->FileDelete(new_filepath);

		App->file_system->FileRename(resultant_filepath, uid);
	}
}

void ResourceAudio::ImportFromLibrary()
{
	std::string resource_filepath = GetLibraryFilepath();

	if (App->file_system->FileExists(resource_filepath))
	{
		audio_sound = App->audio->CreateSound(resource_filepath.c_str());
	}
}

void ResourceAudio::OnRemoveAsset()
{
}

void ResourceAudio::OnRenameAsset(const char * new_name, const char * last_name)
{
}

void ResourceAudio::OnMoveAsset(const char * new_asset_path, const char * last_asset_path)
{
}

bool ResourceAudio::DrawEditorExplorer()
{
	return false;
}

void ResourceAudio::DrawEditorInspector()
{
	if (audio_sound != nullptr)
	{
		if (audio_sound->GetPlaying())
		{
			if (audio_sound->GetPaused())
			{
				if (ImGui::Button("Resume"))
				{
					audio_sound->SetPaused(false);
				}
			}
			else
			{
				if (ImGui::Button("Pause"))
				{
					audio_sound->SetPaused(true);
				}
			}

			ImGui::SameLine();

			if (ImGui::Button("Stop"))
			{
				audio_sound->Stop();
			}
		}
		else
		{
			if (ImGui::Button("Play"))
			{
				audio_sound->Play();
			}
		}
	}
}
