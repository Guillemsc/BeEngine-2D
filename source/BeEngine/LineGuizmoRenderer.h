#ifndef __LINE_GUIZMO_RENDERER_H__
#define __LINE_GUIZMO_RENDERER_H__

#include "ModuleRenderer.h"
#include "MathGeoLib.h"
#include "ModuleShader.h"

class LineGuizmoRenderer
{
public:
	LineGuizmoRenderer();
	~LineGuizmoRenderer();

	void Start();
	void CleanUp();

	void AddLine(float2 start, float2 end);

private:
	ShaderProgram* program = nullptr;
};

#endif // !__LINE_GUIZMO_RENDERER_H__