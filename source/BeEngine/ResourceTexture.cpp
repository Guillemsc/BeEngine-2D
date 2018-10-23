#include "ResourceTexture.h"

ResourceTexture::ResourceTexture(std::string uid) : Resource(uid, ResourceType::TEXTURE)
{
}

void ResourceTexture::CleanUp()
{
}
