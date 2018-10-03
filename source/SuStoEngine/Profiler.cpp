#include "Profiler.h"
#include "Globals.h"
#include "Functions.h"
#include "SDL/include/SDL.h"
#include "gpudetect\DeviceId.h"
#include "mmgr\mmgr.h"

#define MAX_FRAMES_LOGGED 50
#define MAX_MEMORY_LOGGED 50
#define MAX_PROFILE_LOGGED 50

Profiler::Profiler()
{
	time_since_startup.Start();
}

Profiler::~Profiler()
{
}

void Profiler::CleanUp()
{
	for (std::list<Profile*>::iterator it = start_profiles.begin(); it != start_profiles.end(); ++it)
	{
		(*it)->CleanUp();

		RELEASE(*it);
	}

	start_profiles.clear();

	for (std::list<Profile*>::iterator it = update_profiles.begin(); it != update_profiles.end(); ++it)
	{
		(*it)->CleanUp();

		RELEASE(*it);
	}
	
	update_profiles.clear();
}

void Profiler::AppUpdateStart()
{
	// Frame time
	update_ms = frame_time.ReadMs();

	// Cap fps
	if (capped_ms > 0 && update_ms < capped_ms)
	{
		SDL_Delay(capped_ms - update_ms);
	}

	// Frames since start
	frames_since_startup++;

	// Avg fps
	avg_fps = float(frames_since_startup) / time_since_startup.ReadMs();

	// Frames / s
	frame_counter++;
	frame_counter_ms += frame_time.ReadMs();

	if (frame_counter_ms > 1000)
	{
		last_second_frames = frame_counter;
		frame_counter = 0;
		frame_counter_ms = frame_counter_ms - 1000;

		frames.push_back(last_second_frames);
		if (frames.size() > MAX_FRAMES_LOGGED) 
			frames.erase(frames.begin());
		
	}

	frame_time.Start();
	// -----------

	// Memory Statistics
	memory.push_back(m_getMemoryStatistics().totalActualMemory);

	if (memory.size() > MAX_MEMORY_LOGGED) 
		memory.erase(memory.begin());
	
	// -----------
}

Profile * Profiler::AddStartProfile(std::string name)
{
	Profile* profile = new Profile(name);

	start_profiles.push_back(profile);

	return profile;
}

int Profiler::GetStartProfilesCount() const
{
	return start_profiles.size();
}

std::list<Profile*> Profiler::GetStartProfilesList() const
{
	return start_profiles;
}

Profile* Profiler::AddUpdateProfile(std::string name)
{
	Profile* profile = new Profile(name);

	update_profiles.push_back(profile);

	return profile;
}

int Profiler::GetUpdateProfilesCount() const
{
	return update_profiles.size();
}

std::list<Profile*> Profiler::GetUpdateProfilesList() const
{
	return update_profiles;
}

void Profiler::SetMaxFps(int fps)
{
	if (fps > 0)
	{
		max_fps = fps;
		capped_ms = (float)(1000 / (float)fps);
	}
}

const int Profiler::GetMaxFps() const
{
	return max_fps;
}

const float Profiler::GetFrameTime() const
{
	return update_ms;
}

const int Profiler::GetFPS() const
{
	return last_second_frames;
}

const float Profiler::GetAvgFPS() const
{
	return avg_fps*1000;
}

const int Profiler::GetFramesSinceStartup() const
{
	return frames_since_startup;
}

const int Profiler::GetTimeSinceStartup() const
{
	return SDL_GetTicks();
}

std::vector<float> Profiler::GetFramesVector() const
{
	return frames;
}

std::vector<float> Profiler::GetMemoryVector() const
{
	return memory;
}

const int Profiler::GetCpuCount() const
{
	return SDL_GetCPUCount();
}

const int Profiler::GetCpuCacheLineSize() const
{
	return SDL_GetCPUCacheLineSize();
}

const int Profiler::GetSystemRam() const
{
	return SDL_GetSystemRAM();
}

const bool Profiler::Has3DNow() const
{
	return SDL_Has3DNow();
}

const bool Profiler::HasAVX() const
{
	return SDL_HasAVX();
}

const bool Profiler::HasAVX2() const
{
	return SDL_HasAVX2();
}

const bool Profiler::HasAltiVec() const
{
	return SDL_HasAltiVec();
}

const bool Profiler::HasMMX() const
{
	return SDL_HasMMX();
}

const bool Profiler::HasRDTSC() const
{
	return SDL_HasRDTSC();
}

const bool Profiler::HasSSE() const
{
	return SDL_HasSSE();
}

const bool Profiler::HasSSE2() const
{
	return SDL_HasSSE2();
}

const bool Profiler::HasSSE3() const
{
	return SDL_HasSSE3();
}

const bool Profiler::HasSSE41() const
{
	return SDL_HasSSE41();
}

const bool Profiler::HasSSE42() const
{
	return SDL_HasSSE42();
}

Profile::Profile(std::string _name)
{
	name = _name;
}

void Profile::CleanUp()
{
	for (std::list<Profile*>::iterator it = child_profiles.begin(); it != child_profiles.end(); ++it)
	{
		(*it)->CleanUp();

		RELEASE(*it);
	}

	child_profiles.clear();
}

void Profile::Start()
{
	timer.Start();
}

void Profile::Finish()
{
	total_frames_ms += timer.ReadMs();

	ticks.push_back(timer.ReadTicks());

	ms.push_back(timer.ReadMs());

	if (ticks.size() > MAX_PROFILE_LOGGED)
	{
		ticks.erase(ticks.begin());
		ms.erase(ms.begin());
	}
}

const char * Profile::GetName()
{
	return name.c_str();
}

const int Profile::GetLastFrameTick() const
{
	int ret = 0;

	if (!ticks.empty())
	{
		ret = ticks.front();
	}

	return ret;
}

const int Profile::GetLastFrameMs() const
{
	int ret = 0;

	if (!ms.empty())
	{
		ret = ms.front();
	}

	return ret;
}

const std::vector<int> Profile::GetTicksList() const
{
	return ticks;
}

const std::vector<int> Profile::GetMsList() const
{
	return ms;
}

Profile * Profile::AddProfileChild(std::string name)
{
	Profile* profile = new Profile(name);

	child_profiles.push_back(profile);

	return profile;
}

int Profile::GetChildProfilesCount()
{
	return child_profiles.size();
}

std::list<Profile*> Profile::GetChildProfiles() const
{
	return child_profiles;
}
