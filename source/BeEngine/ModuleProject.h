#ifndef __MODULE_PROJECT_H__
#define __MODULE_PROJECT_H__

#include "Module.h"
#include "ModuleThreadTask.h"
#include "ModuleTimeSlicedTask.h"

class Event;

class Project
{
	friend class LoadProjectsThreadTask;
	friend class LoadProjectsTimeSlicedTask;
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
	friend class LoadProjectsThreadTask;
	friend class LoadProjectsTimeSlicedTask;

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
	bool LoadProject(const char* project_folder);
	bool RemoveProject(const char* project_folder);
	void SetCurrProject(Project* set);
	bool GetProjectsLoaded() const;
	std::vector<Project*> GetProjects() const;

private:
	void LoadProjects();
	void SerializeProjects();
	bool ProjectExists(const char* project_path);

	void OnEvent(Event* ev);

private:
	std::vector<Project*> projects;
	bool projects_loaded = false;
	Project* curr_project = nullptr;

	std::string projects_json_filepath;

	LoadProjectsThreadTask* task = nullptr;
	std::mutex projects_lock;

	LoadProjectsTimeSlicedTask* time_sliced_load_projects_task = nullptr;
};

class LoadProjectsThreadTask : public ThreadTask
{
public:
	LoadProjectsThreadTask(ModuleProject* module_proj);

	void Start();
	void Update();
	void Finish();

private:
	ModuleProject* module_proj = nullptr;
};

class LoadProjectsTimeSlicedTask : public TimeSlicedTask
{
public:
	LoadProjectsTimeSlicedTask();

	void Start();
	void Update();
	void Finish();

private:
	JSON_Doc* doc = nullptr;
	int projects_count = 0;
	int curr_project = 0;
};

#endif // !__MODULE_PROJECT_H__