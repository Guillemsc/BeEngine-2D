// ----------------------------------------------------
// j1PerfTimer.cpp
// Slow timer with microsecond precision
// ----------------------------------------------------

#include "PerfTimer.h"
#include "SDL\include\SDL_timer.h"
#include "Globals.h"

int PerfTimer::frequency = 0;

// ---------------------------------------------
PerfTimer::PerfTimer()
{
	Start();
}

// ---------------------------------------------
void PerfTimer::Start()
{
	if (frequency == 0)
		frequency = SDL_GetPerformanceFrequency();

	started_at = SDL_GetPerformanceCounter();
}

void PerfTimer::AddTime(const float& ms)
{
	started_at -= ms;
}

// ---------------------------------------------
const double PerfTimer::ReadMs() const
{
	return 1000.0 * (double(SDL_GetPerformanceCounter() - started_at) / double(frequency));
}

// ---------------------------------------------
const int PerfTimer::ReadTicks() const
{
	return SDL_GetPerformanceCounter() - started_at;
}

