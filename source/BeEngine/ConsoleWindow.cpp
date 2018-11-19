#include "ConsoleWindow.h"
#include "App.h"

ConsoleWindow::ConsoleWindow()
{
}

ConsoleWindow::~ConsoleWindow()
{
}

void ConsoleWindow::Start()
{
}

void ConsoleWindow::CleanUp()
{
}

void ConsoleWindow::DrawEditor()
{
	float2 window_size = GetWindowSize();

	std::vector<ConsoleLogLine> lines_to_add = App->GetConsoleLogs();
	App->ClearConsoleLogs();

	for (std::vector<ConsoleLogLine>::iterator it = lines_to_add.begin(); it != lines_to_add.end(); ++it)
		AddLog((*it));

	if (update_shown_messages)
	{
		UpdateShownMessages();

		update_shown_messages = false;
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::SmallButton("Clear"))
		{
			Clear();
		}

		if (ImGui::Checkbox("Clear on Play", &clear_on_play))
		{
			
		}
		
		if (ImGui::Checkbox("Messages", &log_messages))
		{
			update_shown_messages = true;
		}

		if (ImGui::Checkbox("Warnings", &log_warnings))
		{
			update_shown_messages = true;
		}

		if (ImGui::Checkbox("Errors", &log_errors))
		{
			update_shown_messages = true;
		}
	
		ImGui::EndMenuBar();
	}

	DrawLogs();
}

ImGuiWindowFlags ConsoleWindow::GetWindowFlags()
{
	return ImGuiWindowFlags_MenuBar;
}

void ConsoleWindow::AddLog(const ConsoleLogLine & log)
{
	console_logs.push_back(log);

	if (CanLogType(log.GetType()))
		console_logs_shown.push_back(log);
}

void ConsoleWindow::AddLog(const char * message, const ConsoleLogType & type)
{
	ConsoleLogLine cl(message, type);

	AddLog(cl);
}

void ConsoleWindow::Clear()
{
	console_logs.clear();
	console_logs_shown.clear();
}

void ConsoleWindow::UpdateShownMessages()
{
	console_logs_shown.clear();

	for (std::vector<ConsoleLogLine>::iterator it = console_logs.begin(); it != console_logs.end(); ++it)
	{
		if (CanLogType((*it).GetType()))
			console_logs_shown.push_back((*it));
	}
}

bool ConsoleWindow::CanLogType(const ConsoleLogType & type)
{
	bool ret = false;

	if (log_messages && type == ConsoleLogType::INTERNAL_LOG_INFO)
		ret = true;

	if (log_warnings && type == ConsoleLogType::INTERNAL_LOG_WARNING)
		ret = true;

	if (log_errors && type == ConsoleLogType::INTERNAL_LOG_ERROR)
		ret = true;

	return ret;
}

void ConsoleWindow::DrawLogs()
{
	for (std::vector<ConsoleLogLine>::iterator it = console_logs_shown.begin(); it != console_logs_shown.end(); ++it)
	{
		switch ((*it).GetType())
		{
		case ConsoleLogType::INTERNAL_LOG_INFO:
			ImGui::TextColored(ImVec4(1, 1, 1, 1), (*it).GetLogMessage().c_str());
			break;
		case ConsoleLogType::INTERNAL_LOG_WARNING:
			ImGui::TextColored(ImVec4(255.0f / 255.0f, 153.0f / 255.0f, 55.0f / 255.0f, 1), (*it).GetLogMessage().c_str());
			break;
		case ConsoleLogType::INTERNAL_LOG_ERROR:
			ImGui::TextColored(ImVec4(255.0f /255.0f, 55.0f/255.0f, 55.0f /255.0f, 1), (*it).GetLogMessage().c_str());
			break;
		default:
			break;
		}
	}
}
