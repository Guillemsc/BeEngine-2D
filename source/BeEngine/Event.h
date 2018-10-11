#ifndef __EVENT_H__
#define __EVENT_H__

enum EventType;

class Event
{
public:
	Event() {};

	EventType GetType() const
	{
		return type;
	};

private:
	EventType type;
};

enum EventType
{
	TEST,
};

#endif // !__EVENT_H__