#ifndef __PROJECT_MANAGER_H__
#define __PROJECT_MANAGER_H__

#include "ModuleEditor.h"

class ProjectManager : public EditorElement
{
	friend class ModuleEditor;

private:
	void operator delete(void *) {}

public:
	ProjectManager();
	~ProjectManager();

	void Start();
	void CleanUp();
	void DrawEditor();

private:
	ImFont * big_font = nullptr;
	ImFont * medium_font = nullptr;
	ImFont * small_font = nullptr;
};

#endif // !__PROJECT_MANAGER_H__