#include "ResourceTexture.h"
#include "App.h"
#include "ModuleRenderer.h"
#include "ModuleAssets.h"
#include "ModuleResource.h"
#include "ModuleFileSystem.h"
#include "imgui.h"
#include "ModuleGameObject.h"
#include "GameObject.h"
#include "ComponentSpriteRenderer.h"
#include "ModuleTexture.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

ResourceTexture::ResourceTexture() : Resource(ResourceType::RESOURCE_TYPE_TEXTURE)
{
}

void ResourceTexture::CleanUp()
{
	App->texture->UnloadTexture(texture);
}

bool ResourceTexture::ExistsOnLibrary(std::string uid, std::string & library_filepath)
{
	bool ret = false;

	std::string library_path = App->resource->GetLibraryPathFromResourceType(GetType());

	std::string path = library_path + uid + ".dds";

	if (App->file_system->FileExists(path.c_str()))
	{
		library_filepath = path;
		ret = true;
	}

	return ret;
}

void ResourceTexture::ExportToLibrary(std::string uid)
{
	std::string library_path = App->resource->GetLibraryPathFromResourceType(GetType());

	Texture* temp_texture = App->texture->LoadTexture(GetAssetFilepath());

	if (temp_texture != nullptr)
	{
		App->texture->SaveTexture(temp_texture, IL_DDS, library_path, uid, "dds");

		App->texture->UnloadTexture(temp_texture);
	}
}

void ResourceTexture::ImportFromLibrary()
{
	std::string resource_filepath = GetLibraryFilepath();

	if (texture != nullptr)
		App->texture->UnloadTexture(texture);

	texture = App->texture->LoadTexture(resource_filepath);
}

void ResourceTexture::OnRemoveAsset()
{
	
}

void ResourceTexture::OnRenameAsset(const char * new_name, const char * last_name)
{
}

void ResourceTexture::OnMoveAsset(const char * new_asset_path, const char* last_asset_path)
{
}

uint ResourceTexture::GetTextureId() const
{
	uint ret = 0;

	if (texture != nullptr)
		ret = texture->GetId();

	return ret;
}

float ResourceTexture::GetWidthHeightRatio()
{
	float ret = 0.0f;

	if (texture != nullptr)
	{
		ret = texture->GetWidthHeightRatio();
	}

	return ret;
}

float ResourceTexture::GetHeightWidthRatio()
{
	float ret = 0.0f;

	if (texture != nullptr)
	{
		ret = texture->GetHeightWidthRatio();
	}

	return ret;
}

float2 ResourceTexture::GetSize() const
{
	float2 ret = float2::zero;

	if (texture != nullptr)
		ret = texture->GetSize();

	return ret;
}

bool ResourceTexture::DrawEditorExplorer()
{
	bool ret = false;

	if (ImGui::Button("Load to Scene"))
	{
		GameObject* go = App->gameobject->CreateGameObject();
		go->SetName(d_asset_filepath.file_name.c_str());

		ComponentSpriteRenderer* cs = (ComponentSpriteRenderer*)go->CreateComponent(ComponentType::COMPONENT_TYPE_SPRITE_RENDERER);

		if (cs != nullptr)
			cs->SetResourceTexture(this);

		ret = true;
	}

	return ret;
}

void ResourceTexture::DrawEditorInspector()
{
	if (texture != nullptr)
	{
		ImVec2 size = ImGui::GetWindowSize();

		size.x -= 15;

		float ratio = texture->GetHeightWidthRatio();

		float2 image_size = float2(size.x, size.x * ratio);

		ImGui::Image((ImTextureID)texture->GetId(), ImVec2(image_size.x, image_size.y));
	}
}
