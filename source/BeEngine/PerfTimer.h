#ifndef __j1PERFTIMER_H__
#define __j1PERFTIMER_H__

class PerfTimer
{
public:

	// Constructor
	PerfTimer();

	void Start();
	void AddTime(const float& ms);
	const double ReadMs() const;
	const int ReadTicks() const;

private:
	double     started_at = 0;
	static int frequency;
};

#endif //__j1PERFTIMER_H__