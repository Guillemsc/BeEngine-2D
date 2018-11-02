#ifndef __GRID_RENDERER_H__
#define __GRID_RENDERER_H__

#include "Renderer.h"

#include "ModuleRenderer.h"
#include "ModuleShader.h"
#include "VertexBuffer.h"

class GridRenderer : public Renderer
{
public:
	GridRenderer();
	~GridRenderer();

	void Start();
	void CleanUp();
	void Render(const float4x4& view, const float4x4& projection);

private:
	ShaderProgram* program = nullptr;

	uint vao = 0;
	uint vbo = 0;
};

#endif // !__GRID_RENDERER_H__