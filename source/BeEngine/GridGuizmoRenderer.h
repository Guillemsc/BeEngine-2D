#ifndef __GRID_GUIZMO_RENDERER_H__
#define __GRID_GUIZMO_RENDERER_H__

#include "GuizmoRenderer.h"

#include "ModuleRenderer.h"
#include "ModuleShader.h"
#include "VertexBuffer.h"

class GridGuizmoRenderer : public GuizmoRenderer
{
public:
	GridGuizmoRenderer();
	~GridGuizmoRenderer();

	void Start();
	void CleanUp();
	void Render(const float4x4& view, const float4x4& projection);

private:
	ShaderProgram* program = nullptr;

	uint vao = 0;
	uint vbo = 0;
};

#endif // !__GRID_GUIZMO_RENDERER_H__