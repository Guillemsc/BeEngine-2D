#ifndef __QUAD_RENDERER_H__
#define __QUAD_RENDERER_H__

#include "Renderer.h"

#include "ModuleRenderer.h"
#include "ModuleShader.h"
#include "VertexBuffer.h"

class QuadRenderer : public Renderer
{
public:
	QuadRenderer();
	~QuadRenderer();

	void Start();
	void CleanUp();
	void Render(const float4x4& view, const float4x4& projection);

	void DrawQuad(const float2& pos, const float2& size, const float3& colour, float alpha = 1.0f);
	void DrawRotatedQuad(const float2& pos, const float2& size, const float angle_degrees, const float3& colour, float alpha = 1.0f);

private:
	ShaderProgram* program = nullptr;

	VertexBuffer quads_vb;

	uint vbo = 0;

	uint quads_count = 0;
};

#endif // !__LINE_RENDERER_H__