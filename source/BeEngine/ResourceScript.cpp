#include "ResourceScript.h"

ResourceScript::ResourceScript(std::string uid) : Resource(uid, ResourceType::SCRIPT)
{
}

void ResourceScript::CleanUp()
{
}

void ResourceScript::SetData(const char * _script_path, const char * _script_dll_path)
{
	script_path = _script_path;
	script_dll_path = _script_dll_path;

	Compile();
}

void ResourceScript::Compile()
{
}

bool ResourceScript::GetCompiles() const
{
	return compiles;
}

std::vector<std::string> ResourceScript::GetCompileErrors() const
{
	return compile_errors;
}

const char * ResourceScript::GetScriptCode() const
{
	return script_code.c_str();
}

const char * ResourceScript::GetScriptPath() const
{
	return script_path.c_str();
}

const char * ResourceScript::GetScripDLLPath() const
{
	return script_dll_path.c_str();
}
