#ifndef __MODULE_TEXTURE_H__
#define __MODULE_TEXTURE_H__

#include <map>

#include "Module.h"
#include "GeometryMath.h"

#include "Devil\include\il.h"

class Texture
{
	friend class ModuleTexture;

public:
	Texture();

	uint GetId() const;
	float2 GetSize() const;

	float GetWidthHeightRatio();
	float GetHeightWidthRatio();

private:
	uint id = 0;
	float2 size = float2::zero;

	byte* texture_data = nullptr;
	uint texture_data_size = 0;
	uint texture_data_id = 0;
};

class ModuleTexture : public Module
{
public:
	ModuleTexture();
	~ModuleTexture();

	bool Awake();
	bool CleanUp();

	Texture* LoadTexture(const std::string& path);
	void UnloadTexture(Texture* tex);

	bool SaveTexture(Texture* texture, ILenum format, const std::string& path,const std::string& name, const std::string& extension);

private:
	void UnloadAllTextures();

private:
	std::vector<Texture*> textures;
};

#endif // !__MODULE_TEXTURE_H__