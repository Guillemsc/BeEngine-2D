#ifndef __RESOURCE_TEXTURE_H__
#define __RESOURCE_TEXTURE_H__

#include "Resource.h"
#include "Globals.h"

class ResourceTexture : public Resource
{
	friend class ResourceTextureLoader;

private:
	void operator delete(void *) {}

public:
	ResourceTexture(std::string uid);

	void CleanUp();

	void SetData(uint data_id, byte* data, uint data_size);

	uint GetTextureId();

	uint GetDataId() const;
	byte* GetData() const;
	uint GetDataSize() const;

private:
	byte*  texture_data = nullptr;
	uint   texture_data_size = 0;
	uint   texture_data_id = 0;

	uint   texture_id = 0;

	bool   flipped = false;
};

#endif // !__RESOURCE_TEXTURE_H__