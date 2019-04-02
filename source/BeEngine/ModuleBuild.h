#ifndef __MODULE_BUILD_H__
#define __MODULE_BUILD_H__

#include "Module.h"

class ModuleBuild : public Module
{
public:
	ModuleBuild();
	~ModuleBuild();

	bool Awake();
	bool CleanUp();

	bool GenerateBuild(const std::string& folder, std::vector<std::string>& errors);
};

#endif // !__MODULE_BUILD_H__