#ifndef __RESOURCE_TEXTURE_H__
#define __RESOURCE_TEXTURE_H__

#include "Resource.h"

class ResourceTexture : public Resource
{
	friend class ResourceTextureLoader;

private:
	void operator delete(void *) {}

public:
	ResourceTexture(std::string uid);

	void CleanUp();

	uint GetTextureId();

private:
	byte*  texture_data = nullptr;
	uint   texture_data_size = 0;
	uint   texture_id = 0;

	bool   flipped = false;
};

#endif // !__RESOURCE_TEXTURE_H__