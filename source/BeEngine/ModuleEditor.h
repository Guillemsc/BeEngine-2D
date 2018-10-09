#ifndef __MODULE_EDITOR_H__
#define __MODULE_EDITOR_H__

#include "Module.h"

class ModuleEditor : public Module
{
public:
	ModuleEditor();
	~ModuleEditor();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

	void RenderEditor();

private:
	void ImGuiInit();
	void ImGuiNewFrame();
	void ImGuiQuit();
};

#endif // !__MODULE_EDITOR_H__