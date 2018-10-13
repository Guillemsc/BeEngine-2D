#ifndef __MODULE_PROJECT_H__
#define __MODULE_PROJECT_H__

#include "Module.h"

class Project
{
	friend class ModuleProject;

private:
	void operator delete(void *) {}

public:
	Project();
	~Project();

	void SetPath(const char* path);
	void SetName(const char* name);
	void SetLastTimeOpened(const char* date);

	std::string GetPath() const;
	std::string GetName() const;
	std::string GetLastTimeOpened() const;

private:
	std::string path;
	std::string name;
	std::string last_time_opened;
};

class ModuleProject : public Module
{
public:
	ModuleProject();
	~ModuleProject();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

	std::string GetCurrProjectBasePath() const;

	bool CreateNewProject(const char* path, const char* name);
	bool LoadProject(const char* path);
	void SetCurrProject(Project* set);
	std::vector<Project*> GetProjects() const;

private:
	void LoadProjects();
	void SerializeProjects();

private:
	std::vector<Project*> projects;

	Project* curr_project = nullptr;

	std::string projects_json_filepath;
};

#endif // !__MODULE_PROJECT_H__