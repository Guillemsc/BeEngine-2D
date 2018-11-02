#ifndef __LINE_RENDERER_H__
#define __LINE_RENDERER_H__

#include "Renderer.h"

#include "ModuleRenderer.h"
#include "ModuleShader.h"
#include "VertexBuffer.h"

class LineRenderer : public Renderer
{
public:
	LineRenderer();
	~LineRenderer();

	void Start();
	void CleanUp();
	void Render(const float4x4& view, const float4x4& projection);

	void DrawLine(const float2& start, const float2& end, const float3& colour, float tickness = 10);

private:
	ShaderProgram* program = nullptr;

	VertexBuffer lines_vb;

	uint vao = 0;
	uint vbo = 0;

	uint lines_count = 0;
};

#endif // !__LINE_RENDERER_H__