#include "Resource.h"

Resource::Resource(ResourceType resource_type)
{
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
