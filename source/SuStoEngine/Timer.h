#ifndef __TIMER_H__
#define __TIMER_H__

class Timer
{
public:
	Timer();
	~Timer();

	void Start();
	const int Read() const;
	const float ReadSec() const;
	void AddTime(const float& sec);
	void Stop();
	void PauseOn();
	void PauseOff();

	const bool IsActive() const;


private:
	int  started_at	= 0;
	bool paused = false;
	int  paused_at = 0;
	bool active = false;
};

#endif //__TIMER_H__