#include "ResourceScript.h"

ResourceScript::ResourceScript(std::string uid) : Resource(uid, ResourceType::SCRIPT)
{
}

void ResourceScript::CleanUp()
{
}

bool ResourceScript::GetCompiles() const
{
	return compiles;
}
