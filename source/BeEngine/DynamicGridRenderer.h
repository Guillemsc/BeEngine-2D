#ifndef __DYNAMIC_GRID_RENDERER_H__
#define __DYNAMIC_GRID_RENDERER_H__

#include "Renderer.h"

#include "ModuleRenderer.h"
#include "ModuleShader.h"
#include "VertexBuffer.h"

class DynamicGridRenderer : public Renderer
{
public:
	DynamicGridRenderer();
	~DynamicGridRenderer();

	void Start();
	void CleanUp();
	void Render(const float4x4& view, const float4x4& projection);

	void DrawGrid();

private:
	ShaderProgram* program = nullptr;

	uint vao = 0;

	bool needs_to_render = false;
};

#endif // !__DYNAMIC_GRID_RENDERER_H__