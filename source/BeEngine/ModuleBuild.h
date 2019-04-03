#ifndef __MODULE_BUILD_H__
#define __MODULE_BUILD_H__

#include "Module.h"

class Event;
class ResourceScene;

class ModuleBuild : public Module
{
public:
	ModuleBuild();
	~ModuleBuild();

	bool Awake();
	bool Start();
	bool CleanUp();

	void OnLoadProject(JSON_Doc* config);
	void OnLoadBuild(JSON_Doc* config);

	bool GenerateBuild(const std::string& folder, std::vector<std::string>& errors);

	ResourceScene* GetResourceSceneToLoad() const;

private:
	void OnEvent(Event* ev);

	bool CreateBuildFile(const std::string& folder);

private:
	std::string scene_to_load;

	ResourceScene* resource_scene_to_load = nullptr;

};

#endif // !__MODULE_BUILD_H__