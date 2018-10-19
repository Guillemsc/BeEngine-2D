#ifndef __PROFILER_H__
#define __PROFILER_H__

#include <vector>
#include <list>
#include <string>
#include <iostream>
#include "PerfTimer.h"

class Profile
{
	friend class Profiler;

public:
	Profile(std::string name);

	void CleanUp();

	void Start();
	void Finish();

	const char* GetName();

	const int GetLastFrameTick() const;
	const int GetLastFrameMs() const;

	const float GetAverageMs() const;

	Profile* AddProfileChild(std::string name);
	int GetChildProfilesCount();
	std::list<Profile*> GetChildProfiles() const;

private:
	std::string name;
	PerfTimer   timer;

	std::list<Profile*> child_profiles;

	float total_time_second = 0.0f;
	int total_frames_second = 0;

	float last_second_ms = 0.0f;

	int last_tick = 0;
	int last_ms = 0;

	Profiler* profiler = nullptr;
};

class Profiler
{
public:
	Profiler();
	~Profiler();

	void CleanUp();

	void AppUpdateStart();

	Profile* AddStartProfile(std::string name);
	int GetStartProfilesCount() const;
	std::list<Profile*> GetStartProfilesList() const;

	Profile* AddUpdateProfile(std::string name);
	int GetUpdateProfilesCount() const;
	std::list<Profile*> GetUpdateProfilesList() const;

	void SetMaxFps(int fps);
	const int GetMaxFps() const;
	const float GetFrameTime() const;
	const int GetFPS() const;
	const float GetAvgFPS() const;
	const int GetFramesSinceStartup() const;
	const int GetTimeSinceStartup() const;

	std::vector<float> GetFramesVector() const;
	std::vector<float> GetMemoryVector() const;

	const int GetCpuCount() const;
	const int GetCpuCacheLineSize() const;
	const int GetSystemRam() const;
	const bool Has3DNow() const;
	const bool HasAVX() const;
	const bool HasAVX2() const;
	const bool HasAltiVec() const;
	const bool HasMMX() const;
	const bool HasRDTSC() const;
	const bool HasSSE() const;
	const bool HasSSE2() const;
	const bool HasSSE3() const;
	const bool HasSSE41()  const;
	const bool HasSSE42() const;

public:
	bool frame_one_second = false;

private:
	// Start
	float cration_time = 0.0f;

	// Update 
	PerfTimer frame_time;
	PerfTimer time_since_startup;

	int   frames_since_startup = 0;
	float update_ms = 0.0f;
	float avg_fps = 0.0f;
	int	  max_fps = 0;
	float capped_ms = -1;
	std::vector<float> frames;
	std::vector<float> memory;

	int frame_counter = 0;
	float frame_counter_ms = 0.0f;
	int last_second_frames = 0;

	// Profiles
	std::list<Profile*> start_profiles;
	std::list<Profile*> update_profiles;
};

#endif //__PROFILER_H__