#ifndef __TRIANGLE_RENDERER_H__
#define __TRIANGLE_RENDERER_H__

#include "Renderer.h"

#include "ModuleRenderer.h"
#include "ModuleShader.h"
#include "VertexBuffer.h"

class DinamicTriangleRenderer : public Renderer
{
public:
	DinamicTriangleRenderer();
	~DinamicTriangleRenderer();

	void Start();
	void CleanUp();
	void Render(const float4x4& view, const float4x4& projection);

	void DrawTriangle(const float2& pos, const float2& size, float rotation_degrees, const float3& colour, float alpha = 1.0f);

private:
	ShaderProgram* program = nullptr;

	VertexBuffer triangles_vb;

	uint vbo = 0;

	uint triangles_count = 0;
};

#endif // !__LINE_RENDERER_H__