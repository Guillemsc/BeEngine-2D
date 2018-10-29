#include "ResourceTextureLoader.h"
#include "App.h"
#include "ModuleProject.h"
#include "ResourceTexture.h"
#include "ModuleResource.h"
#include "ModuleJson.h"

#include "Devil\include\il.h"
#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"

#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")

ResourceTextureLoader::ResourceTextureLoader() : ResourceLoader(ResourceType::TEXTURE)
{
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
}

void ResourceTextureLoader::Start()
{
}

void ResourceTextureLoader::CleanUp()
{
	ilShutDown();
}

Resource* ResourceTextureLoader::CreateResource(std::string new_uid)
{
	return new ResourceTexture(new_uid);
}

void ResourceTextureLoader::CreateLibraryFolder()
{
	library_path = App->file_system->CreateFolder(App->resource->GetLibraryPath().c_str(), "textures");
}

bool ResourceTextureLoader::ExportAssetToLibrary(DecomposedFilePath d_filepath, std::string new_uid)
{
	bool ret = false;

	ILuint image;
	ilGenImages(1, &image);
	ilBindImage(image);

	if (ilLoad(IL_TYPE_UNKNOWN, d_filepath.file_path.c_str()))
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
			// Crate assets meta
			std::string assets_meta_path = d_filepath.file_path + ".meta";

			if (App->file_system->FileExists(assets_meta_path.c_str()))
				App->file_system->FileDelete(assets_meta_path.c_str());

			JSON_Doc* doc = App->json->CreateJSON(assets_meta_path.c_str());
			if (doc != nullptr)
			{
				doc->SetString("resource", new_uid.c_str());

				doc->Save();
				App->json->UnloadJSON(doc);

				uint size = ilSaveL(IL_DDS, NULL, 0);
				byte* data = new byte[size];

				if (ilSaveL(IL_DDS, data, size) > 0)
				{
					ret = App->file_system->FileSave(library_path.c_str(), (char*)data, new_uid.c_str(), "dds", size);
				}

				RELEASE_ARRAY(data);
			}

			ret = true;
		}
		else
			ret = false;

		ilDeleteImages(1, &data_id);
		ilBindImage(0);
	}

	return ret;
}

bool ResourceTextureLoader::ClearAssetDataFromEngine(DecomposedFilePath d_filepath)
{
	bool ret = false;

	std::string assets_meta_path = d_filepath.file_path + ".meta";

	JSON_Doc* doc = App->json->LoadJSON(assets_meta_path.c_str());

	if (doc != nullptr)
	{
		std::string resource = doc->GetString("resource");
		resource += ".dds";

		std::string resource_filepath = library_path + resource;

		if (App->file_system->FileExists(resource_filepath.c_str()))
			App->file_system->FileDelete(resource_filepath.c_str());

		App->json->UnloadJSON(doc);
		App->file_system->FileDelete(assets_meta_path.c_str());

		ret = true;
	}

	return ret;
}

bool ResourceTextureLoader::DeleteAssetResources(DecomposedFilePath d_filepath)
{
	bool ret = false;

	std::string assets_meta_path = d_filepath.file_path + ".meta";

	JSON_Doc* doc = App->json->LoadJSON(assets_meta_path.c_str());

	if (doc != nullptr)
	{
		std::string resource = doc->GetString("resource");

		ret = App->resource->DestroyResource(resource, ResourceType::TEXTURE);

		App->json->UnloadJSON(doc);

		ret = true;
	}

	return ret;
}

bool ResourceTextureLoader::RenameAsset(DecomposedFilePath d_filepath, const char * new_name)
{
	bool ret = false;

	App->file_system->FileRename(d_filepath.file_path.c_str(), new_name);

	std::string assets_meta_path = d_filepath.file_path + ".meta";

	if (App->file_system->FileExists(assets_meta_path.c_str()))
	{
		std::string meta_new_name = new_name + std::string(".") + d_filepath.file_extension;

		App->file_system->FileRename(assets_meta_path.c_str(), meta_new_name.c_str());
	}

	return ret;
}

bool ResourceTextureLoader::IsAssetOnLibrary(DecomposedFilePath d_filepath, std::vector<std::string>& library_files_used)
{
	bool ret = false;

	std::string assets_meta_path = d_filepath.file_path + ".meta";

	JSON_Doc* doc = App->json->LoadJSON(assets_meta_path.c_str());

	if (doc != nullptr)
	{
		std::string resource = doc->GetString("resource");
		resource += ".dds";

		std::string resource_filepath = library_path + resource;

		if (App->file_system->FileExists(resource_filepath.c_str()))
			ret = true;

		App->json->UnloadJSON(doc);
	}

	return ret;
}

bool ResourceTextureLoader::ImportAssetFromLibrary(DecomposedFilePath d_filepath, std::vector<Resource*>& resources)
{
	bool ret = false;

	std::string assets_meta_path = d_filepath.file_path + ".meta";

	JSON_Doc* doc = App->json->LoadJSON(assets_meta_path.c_str());

	if (doc != nullptr)
	{
		std::string resource = doc->GetString("resource");
		resource += ".dds";

		std::string resource_filepath = library_path + resource;

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
					ResourceTexture* r_tex = (ResourceTexture*)App->resource->CreateResource(ResourceType::TEXTURE);
					r_tex->SetData(data_id, data, data_size);

					ret = true;
				}
				else
					ret = false;

				ilBindImage(0);
			}
		}

		App->json->UnloadJSON(doc);
	}

	return ret;
}

bool ResourceTextureLoader::ExportResourceToLibrary(Resource * resource)
{
	bool ret = false;

	if (resource != nullptr)
	{
		ILuint image;
		ilGenImages(1, &image);
		ilBindImage(image);

		ResourceTexture* resource_txt = (ResourceTexture*)resource;

		ILenum imageType = ilDetermineTypeL(resource_txt->GetData(), resource_txt->GetDataSize());

		ilBindImage(resource_txt->GetDataId());

		ilSetInteger(IL_DXTC_FORMAT, IL_DXT5);

		ilEnable(IL_FILE_OVERWRITE);

		uint size = ilSaveL(IL_DDS, NULL, 0);
		byte* data = new byte[size];

		if (ilSaveL(IL_DDS, data, size) > 0)
		{
			ret = App->file_system->FileSave(library_path.c_str(), (char*)data, resource_txt->GetUID().c_str(), "dds", size);
		}

		RELEASE_ARRAY(data);

		ilBindImage(0);
	}

	return ret;
}

void ResourceTextureLoader::OnDestroyResource(Resource * res)
{
	if (res != nullptr)
	{
		ResourceTexture* resource_txt = (ResourceTexture*)res;

		ILuint id = resource_txt->GetDataId();
		ilDeleteImages(1, &id);
	}
}
