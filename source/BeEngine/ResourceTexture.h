#ifndef __RESOURCE_TEXTURE_H__
#define __RESOURCE_TEXTURE_H__

#include "Resource.h"
#include "Globals.h"
#include "GeometryMath.h"

class Texture;

class ResourceTexture : public Resource
{
	friend class ModuleResource;

private:
	void operator delete(void *) {}

public:
	ResourceTexture();

	void CleanUp();

	bool ExistsOnLibrary(std::string uid, std::string& library_filepath = std::string());
	void ExportToLibrary(std::string uid);
	void ImportFromLibrary();

	void OnRemoveAsset();
	void OnRenameAsset(const char* new_name, const char* last_name);
	void OnMoveAsset(const char* new_asset_path, const char* last_asset_path);

	uint GetTextureId() const;
	float GetWidthHeightRatio();
	float GetHeightWidthRatio();
	float2 GetSize() const;

	bool DrawEditorExplorer();

private:
	Texture* texture = nullptr;

};

#endif // !__RESOURCE_TEXTURE_H__