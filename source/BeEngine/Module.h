#pragma once

class Application;
class JSON_Doc;

#include "SDL/include/SDL.h"

#include <string>

#include "Profiler.h"
#include "Globals.h"

class Module
{
	friend Application;

public:
	Module()
	{}

	virtual ~Module()
	{}

	virtual void CreateProfiles()
	{};

	virtual bool Awake()
	{
		return true;
	}

	virtual bool Start()
	{
		return true;
	}

	virtual bool PreUpdate()
	{
		return true;
	}

	virtual bool Update()
	{
		return true;
	}

	virtual bool PostUpdate()
	{
		return true;
	}

	virtual bool CleanUp()
	{
		return true;
	}

	virtual void OnLoadConfig(JSON_Doc* config) {};
	virtual void OnSaveConfig(JSON_Doc* config) {};

	virtual void OnLoadProject(JSON_Doc* config) {};
	virtual void OnLoadBuild(JSON_Doc* config) {};

	virtual void OnLoadFile(const char* filepath) {};

	void SetName(const char* set_name) 
	{ 
		name = set_name; 
	}

	const char* GetName() 
	{ 
		return name.c_str(); 
	}

	bool GetEnabled() 
	{ 
		return enabled; 
	}

	void SetEnabled(bool set) 
	{ 
		enabled = set; 
	}

protected:
	Profile* prof_module_awake = nullptr;
	Profile* prof_module_start = nullptr;
	Profile* prof_module_preupdate = nullptr;
	Profile* prof_module_update = nullptr;
	Profile* prof_module_postupdate = nullptr;

private:
	std::string name = "";
	bool        enabled = true;
};