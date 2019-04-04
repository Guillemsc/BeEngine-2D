#include "ConsoleWindow.h"
#include "App.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

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
	return ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_HorizontalScrollbar;
}

void ConsoleWindow::AddConsolePersonalLogs(const std::string & identifier)
{
	bool exists = false;
	for (std::vector<ConsolePersonalLogs>::iterator it = personal_logs.begin(); it != personal_logs.end(); ++it)
	{
		if ((*it).personal_identifier.compare(identifier) == 0)
		{
			exists = true;
			break;
		}
	}

	if (!exists)
	{
		ConsolePersonalLogs pl;
		pl.personal_identifier = identifier;

		personal_logs.push_back(pl);
	}

}

void ConsoleWindow::AddPersonalLog(const std::string & identifier, const char * message, const ConsoleLogType & type)
{
	for (std::vector<ConsolePersonalLogs>::iterator it = personal_logs.begin(); it != personal_logs.end(); ++it)
	{
		if ((*it).personal_identifier.compare(identifier) == 0)
		{
			ConsoleLogLine cl(message, type);
			(*it).logs.push_back(cl);

			if (CanLogType(type))
			{
				(*it).logs_shown.push_back(cl);
			}
		}
	}
}

void ConsoleWindow::ClearPesonalLogs(const std::string & identifier)
{
	for (std::vector<ConsolePersonalLogs>::iterator it = personal_logs.begin(); it != personal_logs.end(); ++it)
	{
		if ((*it).personal_identifier.compare(identifier) == 0)
		{
			(*it).logs.clear();
			(*it).logs_shown.clear();
		}
	}
}

void ConsoleWindow::AddLog(const ConsoleLogLine & log)
{
	console_logs.push_back(log);

	if (CanLogType(log.GetType()))
	{
		if (console_logs_shown.size() > 100)
			console_logs_shown.erase(console_logs_shown.begin());

		console_logs_shown.push_back(log);
	}
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

	for (std::vector<ConsolePersonalLogs>::iterator it = personal_logs.begin(); it != personal_logs.end(); ++it)
	{
		(*it).logs_shown.clear();

		for (std::vector<ConsoleLogLine>::iterator l = (*it).logs.begin(); l != (*it).logs.end(); ++l)
		{
			if (CanLogType((*l).GetType()))
				(*it).logs_shown.push_back(*l);
		}
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

bool ConsoleWindow::PersonalLogExists(const std::string & identifier)
{
	bool ret = false;

	for (std::vector<ConsolePersonalLogs>::iterator it = personal_logs.begin(); it != personal_logs.end(); ++it)
	{
		if ((*it).personal_identifier.compare(identifier) == 0)
		{
			ret = true;
			break;
		}
	}

	return ret;
}

void ConsoleWindow::DrawLogs()
{
	// Normal logs
	for (std::vector<ConsoleLogLine>::reverse_iterator it = console_logs_shown.rbegin(); it != console_logs_shown.rend(); ++it)
	{
		DrawLog(*it);
	}

	// Personal logs
	for (std::vector<ConsolePersonalLogs>::iterator it = personal_logs.begin(); it != personal_logs.end(); ++it)
	{
		for (std::vector<ConsoleLogLine>::reverse_iterator l = (*it).logs_shown.rbegin(); l != (*it).logs_shown.rend(); ++l)
		{
			DrawLog(*l);
		}
	}
}

ConsoleLogLine ConsoleWindow::GetLastLog() const
{
	ConsoleLogLine ret;

	if (console_logs.size() > 0)
		ret = *console_logs.rbegin();

	for (std::vector<ConsolePersonalLogs>::const_iterator it = personal_logs.begin(); it != personal_logs.end(); ++it)
	{
		if ((*it).logs.size() > 0)
		{
			ret = *(*it).logs.rbegin();

			break;
		}
	}

	return ret;
}

void ConsoleWindow::DrawLog(const ConsoleLogLine & log)
{
	switch (log.GetType())
	{
	case ConsoleLogType::INTERNAL_LOG_INFO:
		ImGui::TextColored(ImVec4(1, 1, 1, 1), log.GetLogMessage().c_str());
		break;
	case ConsoleLogType::INTERNAL_LOG_WARNING:
		ImGui::TextColored(ImVec4(255.0f / 255.0f, 153.0f / 255.0f, 55.0f / 255.0f, 1), log.GetLogMessage().c_str());
		break;
	case ConsoleLogType::INTERNAL_LOG_ERROR:
		ImGui::TextColored(ImVec4(255.0f / 255.0f, 55.0f / 255.0f, 55.0f / 255.0f, 1), log.GetLogMessage().c_str());
		break;
	default:
		break;
	}
}

ConsolePersonalLogs::ConsolePersonalLogs()
{
}
