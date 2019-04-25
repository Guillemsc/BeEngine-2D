#ifndef __RESOURCE_AUDIO_H__
#define __RESOURCE_AUDIO_H__

#include "Resource.h"
#include "Globals.h"
#include "ModuleScripting.h"
#include "ModuleText.h"

class GameObject;
class GameObjectComponent;
class AudioSound;

class ResourceAudio : public Resource
{
	friend class ModuleResource;
	friend class ModuleAssets;

private:
	void operator delete(void *) {}

public:
	ResourceAudio();

	void CleanUp();

	bool ExistsOnLibrary(std::string uid, std::string& library_filepath = std::string());
	void ExportToLibrary(std::string uid);
	void ImportFromLibrary();

	void OnRemoveAsset();
	void OnRenameAsset(const char* new_name, const char* last_name);
	void OnMoveAsset(const char* new_asset_path, const char* last_asset_path);

	bool DrawEditorExplorer();
	void DrawEditorInspector();

private:
	AudioSound* audio_sound = nullptr;

};

#endif // !__RESOURCE_AUDIO_H__