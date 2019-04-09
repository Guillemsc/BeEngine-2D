#include "ModuleTexture.h"
#include "App.h"
#include "ModuleFileSystem.h"
#include "ModuleRenderer.h"

#include "Devil\include\ilu.h"
#include "Devil\include\ilut.h"

#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")

Texture::Texture()
{
}

uint Texture::GetId() const
{
	return id;
}

float2 Texture::GetSize() const
{
	return size;
}

ModuleTexture::ModuleTexture()
{
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
}

ModuleTexture::~ModuleTexture()
{
}

bool ModuleTexture::Awake()
{
	bool ret = true;



	return ret;
}

bool ModuleTexture::CleanUp()
{
	bool ret = true;

	UnloadAllTextures();

	ilShutDown();

	return ret;
}

Texture* ModuleTexture::LoadTexture(const std::string & path)
{
	Texture* ret = nullptr;
	
	if (App->file_system->FileExists(path))
	{
		ret = new Texture();

		ILuint image;
		ilGenImages(1, &image);
		ilBindImage(image);

		if (ilLoad(IL_TYPE_UNKNOWN, path.c_str()))
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

			ret->id = App->renderer->LoadTextureToVRAM(image_width, image_height, data, format);

			if (ret->id > 0)
			{
				ret->texture_data_id = data_id;
				ret->texture_data = data;
				ret->texture_data_size = data_size;

				ret->size.x = image_width;
				ret->size.y = image_height;
			}

			ilBindImage(0);
		}

		textures.push_back(ret);
	}
	
	return ret;
}

void ModuleTexture::UnloadTexture(Texture* tex)
{
	if (tex != nullptr)
	{
		bool found = false;

		for (std::vector<Texture*>::iterator it = textures.begin(); it != textures.end(); ++it)
		{
			if ((*it) == tex)
			{
				textures.erase(it);
				found = true;
				break;
			}
		}

		if (found)
		{
			App->renderer->DeleteTexture(tex->id);

			ilDeleteImages(1, &tex->texture_data_id);

			RELEASE(tex);
		}
	}
}

bool ModuleTexture::SaveTexture(Texture* texture, ILenum format, const std::string& path, const std::string& name, const std::string& extension)
{
	bool ret = false;

	if (texture->texture_data_size > 0)
	{
		uint size = ilSaveL(format, NULL, 0);
		byte* data = new byte[size];

		if (ilSaveL(format, data, size) > 0)
		{
			App->file_system->FileSave(path, name, extension, (char*)texture->texture_data, size);
		}
	}

	return ret;
}

void ModuleTexture::UnloadAllTextures()
{
	for (std::vector<Texture*>::iterator it = textures.begin(); it != textures.end(); ++it)
	{		
		App->renderer->DeleteTexture((*it)->id);

		ilDeleteImages(1, &(*it)->texture_data_id);

		RELEASE((*it));
	}

	textures.clear();
}


