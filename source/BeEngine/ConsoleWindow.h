#ifndef __CONSOLE_WINDOW_H__
#define __CONSOLE_WINDOW_H__

#include "ModuleEditor.h"
#include "imgui.h"

class ConsoleWindow : public EditorWindow
{
	friend class ModuleEditor;

private:
	void operator delete(void *) {}

public:
	ConsoleWindow();
	~ConsoleWindow();

	void Start();
	void CleanUp();
	void DrawEditor();
	ImGuiWindowFlags GetWindowFlags();

	void AddLog(const ConsoleLogLine& log);
	void AddLog(const char* message, const ConsoleLogType& type);
	void Clear();

private:
	void UpdateShownMessages();
	bool CanLogType(const ConsoleLogType& type);
	void DrawLogs();

private:
	ImFont* font = nullptr;

	std::vector<ConsoleLogLine> console_logs;

	std::vector<ConsoleLogLine> console_logs_shown;

	bool clear_on_play = false;

	bool log_messages = true;
	bool log_warnings = true;
	bool log_errors = true;

	bool update_shown_messages;
};

#endif // !__HIERARCHY_WINDOW_H__