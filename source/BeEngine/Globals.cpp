#include "Globals.h"
#include "App.h"

void InternalLog(const char file[], int line, const char * format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;

	// Construct the std::string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", file, line, tmp_string);
	OutputDebugString(tmp_string2);

	if (App != nullptr)
		App->AddInternalLog(tmp_string);
}

void ConsoleLog(ConsoleLogType type, const char file[], int line, const char * format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;

	// Construct the std::string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", file, line, tmp_string);

	ConsoleLogLine log(tmp_string, type);

	if (App != nullptr)
		App->AddConsoleLog(log);
}

ConsoleLogLine::ConsoleLogLine()
{
}

ConsoleLogLine::ConsoleLogLine(const char * _message, const ConsoleLogType & _type)
{
	message = _message;
	type = _type;
}

std::string ConsoleLogLine::GetLogMessage() const
{
	return message;
}

ConsoleLogType ConsoleLogLine::GetType() const
{
	return type;
}
