#ifndef __TOOLS_BAR_H__
#define __TOOLS_BAR_H__

#include "ModuleEditor.h"
#include "GeometryMath.h"

class ToolsBar : public EditorElement
{
	friend class ModuleEditor;

private:
	void operator delete(void *) {}

public:
	ToolsBar(float2 margins_left_up);
	~ToolsBar();

	void CleanUp();
	void DrawEditor();

private:
	float2 margins_left_up = float2::zero;
};

#endif // !__TOOLS_BAR_H__