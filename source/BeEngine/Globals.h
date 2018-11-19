#ifndef __GLOBALS_H__
#define __GLOBALS_H__

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 )

#include <windows.h>
#include <stdio.h>
#include <string>

typedef unsigned int uint;
typedef unsigned char uchar;

enum ConsoleLogType
{
	INTERNAL_LOG_INFO,
	INTERNAL_LOG_WARNING,
	INTERNAL_LOG_ERROR,
};

class ConsoleLogLine
{
public:
	ConsoleLogLine();
	ConsoleLogLine(const char* message, const ConsoleLogType& type);

	std::string GetLogMessage() const;
	ConsoleLogType GetType() const;

private:
	std::string message;
	ConsoleLogType type = ConsoleLogType::INTERNAL_LOG_INFO;
};

// Deletes a buffer
#define RELEASE( x )		\
    {                       \
    if( x != nullptr )      \
	    {                   \
      delete x;             \
	  x = nullptr;          \
	    }                   \
    }

// Deletes an array of buffers
#define RELEASE_ARRAY( x )  \
    {                       \
    if( x != nullptr )      \
	    {                   \
      delete[] x;           \
	  x = nullptr;          \
	    }                   \
                            \
    }

void InternalLog(const char file[], int line, const char* format, ...);
void ConsoleLog(ConsoleLogType type, const char file[], int line, const char* format, ...);

#define INTERNAL_LOG(format, ...) InternalLog(__FILE__, __LINE__, format, __VA_ARGS__);

#define CONSOLE_INFO(format, ...) ConsoleLog(ConsoleLogType::INTERNAL_LOG_INFO, __FILE__, __LINE__, format, __VA_ARGS__);
#define CONSOLE_WARNING(format, ...) ConsoleLog(ConsoleLogType::INTERNAL_LOG_WARNING, __FILE__, __LINE__, format, __VA_ARGS__);
#define CONSOLE_ERROR(format, ...) ConsoleLog(ConsoleLogType::INTERNAL_LOG_ERROR, __FILE__, __LINE__, format, __VA_ARGS__);

#define DEGTORAD   0.0174532925199432957f
#define RADTODEG   57.295779513082320876f
#define PI		   3.14159265358979323846f
#define TWO_PI	   6.28318530717958647692f
#define HALF_PI	   1.57079632679489661923f
#define QUARTER_PI 0.78539816339744830961f
#define INV_PI	   0.31830988618379067154f
#define INV_TWO_PI 0.15915494309189533576f

#endif // !__GLOBALS_H__

