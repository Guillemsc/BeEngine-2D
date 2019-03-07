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

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

ResourceTexture::ResourceTexture() : Resource(ResourceType::RESOURCE_TYPE_TEXTURE)
{
}

void ResourceTexture::CleanUp()
{
	ilDeleteImages(1, &texture_data_id);
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

	std::string filepath = library_path + uid + ".dds";

	ILuint image;
	ilGenImages(1, &image);
	ilBindImage(image);

	if (ilLoad(IL_TYPE_UNKNOWN, GetAssetFilepath().c_str()))
	{
		// Get texture info
		ILinfo ImageInfo;
		iluGetImageInfo(&ImageInfo);

		if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
		{
			iluFlipImage();
		}

		// Convert image to rgb and a byte chain
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		ILubyte* data = ilGetData();
		uint data_id = ImageInfo.Id;
		uint data_size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
		uint image_width = ilGetInteger(IL_IMAGE_WIDTH);
		uint image_height = ilGetInteger(IL_IMAGE_HEIGHT);
		uint format = ilGetInteger(IL_IMAGE_FORMAT);
		uint type = ilGetInteger(IL_IMAGE_TYPE);

		if (data_size > 0)
		{
			uint size = ilSaveL(IL_DDS, NULL, 0);
			byte* data = new byte[size];

			if (ilSaveL(IL_DDS, data, size) > 0)
			{
				App->file_system->FileSave(library_path.c_str(), uid.c_str(), "dds", (char*)data, size);
			}

			RELEASE_ARRAY(data);

		}

		ilDeleteImages(1, &data_id);
		ilBindImage(0);
	}
}

void ResourceTexture::ImportFromLibrary()
{
	std::string resource_filepath = GetLibraryFilepath();

	if (App->file_system->FileExists(resource_filepath.c_str()))
	{
		ILuint image;
		ilGenImages(1, &image);
		ilBindImage(image);

		if (ilLoad(IL_TYPE_UNKNOWN, resource_filepath.c_str()))
		{
			// Get texture info
			ILinfo ImageInfo;
			iluGetImageInfo(&ImageInfo);

			if (ImageInfo.Origin != IL_ORIGIN_UPPER_LEFT)
			{
				iluFlipImage();
			}

			// Convert image to rgb and a byte chain
			ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

			ILubyte* data = ilGetData();
			uint data_id = ImageInfo.Id;
			uint data_size = ilGetInteger(IL_IMAGE_SIZE_OF_DATA);
			uint image_width = ilGetInteger(IL_IMAGE_WIDTH);
			uint image_height = ilGetInteger(IL_IMAGE_HEIGHT);
			uint format = ilGetInteger(IL_IMAGE_FORMAT);
			uint type = ilGetInteger(IL_IMAGE_TYPE);

			if (texture_id != 0)
				App->renderer->DeleteTexture(texture_id);

			texture_id = App->renderer->LoadTextureToVRAM(image_width, image_height, data, format);

			if (data_size > 0)
			{
				texture_data_id = data_id;
				texture_data = data;
				texture_data_size = data_size;

				texture_width = image_width;
				texture_height = image_height;
			}

			ilBindImage(0);
		}
	}
}

void ResourceTexture::OnRemoveAsset()
{
	if (texture_id != 0)
		App->renderer->DeleteTexture(texture_id);
}

void ResourceTexture::OnRenameAsset(const char * new_name, const char * last_name)
{
}

void ResourceTexture::OnMoveAsset(const char * new_asset_path, const char* last_asset_path)
{
}

uint ResourceTexture::GetTextureId() const
{
	return texture_id;
}

float ResourceTexture::GetWidthHeightRatio()
{
	float ret = 0.0f;

	if(texture_height != 0)
		ret = (float)texture_width / (float)texture_height;

	return ret;
}

float ResourceTexture::GetHeightWidthRatio()
{
	float ret = 0.0f;

	if (texture_width != 0)
		ret = (float)texture_height / (float)texture_width;

	return ret;
}

float2 ResourceTexture::GetSize() const
{
	return float2(texture_width, texture_height);
}

uint ResourceTexture::GetDataId() const
{
	return texture_data_id;
}

byte * ResourceTexture::GetData() const
{
	return texture_data;
}

uint ResourceTexture::GetDataSize() const
{
	return texture_data_size;
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
