#ifndef __TOOLS_BAR_H__
#define __TOOLS_BAR_H__

#include "ModuleEditor.h"
#include "GeometryMath.h"

class Texture;

class ToolsBar : public EditorElement
{
	friend class ModuleEditor;

private:
	void operator delete(void *) {}

public:
	ToolsBar(float2 margins_left_up);
	~ToolsBar();

	void Start();
	void CleanUp();
	void DrawEditor();

private:
	float2 margins_left_up = float2::zero;

	Texture* play_texture = nullptr;
	Texture* pause_texture = nullptr;
};

#endif // !__TOOLS_BAR_H__