#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include <string>

enum ResourceType
{
	UNKWNOWN,
	TEXTURE,
	SCRIPT,
	SHADER,
};

class Resource
{
	friend class ResourceLoader;
	friend class ModuleResource;

private:
	void operator delete(void *) {}

public:
	Resource(std::string uid, ResourceType resource_type);

	const ResourceType GetType() const;
	std::string GetUID() const;

	virtual void CleanUp();

private:
	ResourceType type = ResourceType::UNKWNOWN;

	std::string uid;
};

#endif // !__RESOURCE_H__