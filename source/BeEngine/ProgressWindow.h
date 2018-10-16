#ifndef __PROGRESS_WINDOW_H__
#define __PROGRESS_WINDOW_H__

#include "ModuleEditor.h"
#include <map>

class ProgressWindow : public EditorElement
{
	friend class ModuleEditor;

private:
	void operator delete(void *) {}

public:
	ProgressWindow();
	~ProgressWindow();

	void Start();
	void CleanUp();
	void DrawEditor();

	void AddProcess(std::string name, float process);

private:
	ImFont* font = nullptr;
	float2 last_window_size = float2::zero;

	std::map<std::string, float> processes;

};

#endif // !__PROGRESS_WINDOW_H__