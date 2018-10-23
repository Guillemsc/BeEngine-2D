#include "Resource.h"

Resource::Resource(std::string _uid, ResourceType resource_type)
{
	uid = _uid;
	type = resource_type;
}

const ResourceType Resource::GetType() const
{
	return type;
}

std::string Resource::GetUID() const
{
	return uid;
}

void Resource::CleanUp()
{
}
