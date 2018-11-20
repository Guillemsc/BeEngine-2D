#ifndef __CONSOLE_WINDOW_H__
#define __CONSOLE_WINDOW_H__

#include "ModuleEditor.h"
#include "imgui.h"

class ConsolePersonalLogs
{
public:
	ConsolePersonalLogs();

	std::string personal_identifier;

	std::vector<ConsoleLogLine> logs;
	std::vector<ConsoleLogLine> logs_shown;
};

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

	void AddConsolePersonalLogs(const std::string& identifier);

	void AddPersonalLog(const std::string& identifier, const char* message, const ConsoleLogType& type);
	void ClearPesonalLogs(const std::string& identifier);

	void AddLog(const ConsoleLogLine& log);
	void AddLog(const char* message, const ConsoleLogType& type);
	void Clear();

private:
	void UpdateShownMessages();

	bool CanLogType(const ConsoleLogType& type);

	bool PersonalLogExists(const std::string& identifier);

	void DrawLogs();
	void DrawLog(const ConsoleLogLine& log);
private:
	ImFont* font = nullptr;

	std::vector<ConsoleLogLine> console_logs;

	std::vector<ConsoleLogLine> console_logs_shown;

	std::vector<ConsolePersonalLogs> personal_logs;
	std::map<std::string, ConsoleLogLine> personal_logs_to_add;
	std::vector<std::string> personal_logs_to_clear;

	bool clear_on_play = false;

	bool log_messages = true;
	bool log_warnings = true;
	bool log_errors = true;

	bool update_shown_messages = false;
};

#endif // !__HIERARCHY_WINDOW_H__