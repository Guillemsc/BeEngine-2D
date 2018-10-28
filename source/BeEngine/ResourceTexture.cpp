#include "ResourceTexture.h"
#include "ResourceLoader.h"

ResourceTexture::ResourceTexture(std::string uid) : Resource(uid, ResourceType::TEXTURE)
{
}

void ResourceTexture::CleanUp()
{

}

void ResourceTexture::SetData(uint data_id, byte * data, uint data_size)
{
	texture_data_id = data_id;
	texture_data = data;
	texture_data_size = data_size;
}

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
