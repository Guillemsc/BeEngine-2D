#ifndef __LINE_GUIZMO_RENDERER_H__
#define __LINE_GUIZMO_RENDERER_H__

#include "GuizmoRenderer.h"

#include "ModuleRenderer.h"
#include "ModuleShader.h"
#include "VertexBuffer.h"

class LineGuizmoRenderer : public GuizmoRenderer
{
public:
	LineGuizmoRenderer();
	~LineGuizmoRenderer();

	void Start();
	void CleanUp();
	void Render(const float4x4& view, const float4x4& projection);

	void DrawLine(float2 start, float2 end, float3 colour);

private:
	ShaderProgram* program = nullptr;

	VertexBuffer lines_vb;

	uint vao = 0;
	uint vbo = 0;

	uint lines_count = 0;
};

#endif // !__LINE_GUIZMO_RENDERER_H__