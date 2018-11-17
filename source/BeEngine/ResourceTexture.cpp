#include "ResourceTexture.h"
#include "ResourceLoader.h"
#include "App.h"
#include "ModuleResource.h"

ResourceTexture::ResourceTexture() : Resource(ResourceType::TEXTURE)
{
}

void ResourceTexture::CleanUp()
{

}

bool ResourceTexture::OnExistsOnLibrary(std::string uid, std::string & library_filepath)
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

void ResourceTexture::OnExportToLibrary(std::string uid)
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
				App->file_system->FileSave(library_path.c_str(), (char*)data, uid.c_str(), "dds", size);
			}

			RELEASE_ARRAY(data);

		}

		ilDeleteImages(1, &data_id);
		ilBindImage(0);
	}
}

void ResourceTexture::OnImportFromLibrary()
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
				texture_data_id = data_id;
				texture_data = data;
				texture_data_size = data_size;
			}

			ilBindImage(0);
		}
	}
}
//
//void ResourceTexture::SetData(uint data_id, byte * data, uint data_size)
//{
//	texture_data_id = data_id;
//	texture_data = data;
//	texture_data_size = data_size;
//}

uint ResourceTexture::GetTextureId()
{
	return uint();
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
