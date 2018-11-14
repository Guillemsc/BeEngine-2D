#ifndef __RESOURCE_SCRIPT_H__
#define __RESOURCE_SCRIPT_H__

#include "Resource.h"
#include "Globals.h"
#include <vector>

class ResourceScript : public Resource
{
	friend class ResourceScriptLoader;

private:
	void operator delete(void *) {}

public:
	ResourceScript(std::string uid);

	void CleanUp();

	void SetData(const char* script_path, const char* script_dll_path);

	void Compile();

	bool GetCompiles() const;
	std::vector<std::string> GetCompileErrors() const;
	const char* GetScriptCode() const;
	const char* GetScriptPath() const;
	const char* GetScripDLLPath() const;

private:
	bool compiles = false;
	std::string script_path;
	std::string script_dll_path;
	std::string script_code;

	std::vector<std::string> compile_errors;
};

#endif // !__RESOURCE_SCRIPT_H__