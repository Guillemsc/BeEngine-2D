#ifndef __MODULE_TIME_SLICED_TASK_H__
#define __MODULE_TIME_SLICED_TASK_H__

#include "Module.h"
#include<functional>

enum TimeSlicedTaskType
{
	FOCUS,
	BACKGROUND,
};

class TimeSlicedTask
{
	friend class ModuleTimeSlicedTask;

public:
	TimeSlicedTask(TimeSlicedTaskType mode, uint iterations_per_frame = 10, std::string task_name = "TimeSlicedTask");

	virtual void Start() {};
	virtual void Update() {};
	virtual void Finish() {};

	void SetPercentageProgress(float set);
	float GetPercentageProgress() const;

	void SetDescription(const std::string& phase);
	std::string GetDescription() const;

	void FinishTask();
	TimeSlicedTaskType GetMode() const;

	void OnStart(const std::function<void(TimeSlicedTask*)>& fun);
	void OnUpdate(const std::function<void(TimeSlicedTask*)>& fun);
	void OnFinish(const std::function<void(TimeSlicedTask*)>& fun);

private:
	TimeSlicedTaskType mode = TimeSlicedTaskType::FOCUS;
	bool		       finished = false;
	uint		       iterations_per_frame = 0;
	std::string	       task_name;
				       
	float		       progress = 0.0f;
	std::string	       description;
				       
	bool		       first_update = true;

	std::function<void(TimeSlicedTask*)> on_start;
	std::function<void(TimeSlicedTask*)> on_update;
	std::function<void(TimeSlicedTask*)> on_finish;
};

class ModuleTimeSlicedTask : public Module
{
public:
	ModuleTimeSlicedTask();
	~ModuleTimeSlicedTask();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

	void StartTimeSlicedTask(TimeSlicedTask* task);
	void FinishTimeSlicedTask(TimeSlicedTask* task);

private:
	void UpdateTimeSlicedTasks();
	void UpdateEditor();

private:
	std::vector<TimeSlicedTask*> running_tasks;
};

#endif // !__MODULE_TIME_SLICED_TASK_H__