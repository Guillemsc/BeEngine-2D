#ifndef __RESOURCE_SCRIPT_H__
#define __RESOURCE_SCRIPT_H__

#include "Resource.h"
#include "Globals.h"

class ResourceScript : public Resource
{
	friend class ResourceScriptLoader;

private:
	void operator delete(void *) {}

public:
	ResourceScript(std::string uid);

	void CleanUp();

	bool GetCompiles() const;

private:
	bool compiles = false;
	std::string script_path;
};

#endif // !__RESOURCE_SCRIPT_H__