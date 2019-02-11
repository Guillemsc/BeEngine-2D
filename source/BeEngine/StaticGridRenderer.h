#ifndef __STATIC_GRID_RENDERER_H__
#define __STATIC_GRID_RENDERER_H__

#include "Renderer.h"

#include "ModuleRenderer.h"
#include "ModuleShader.h"
#include "VertexBuffer.h"

class StaticGridRenderer : public Renderer
{
public:
	StaticGridRenderer();
	~StaticGridRenderer();

	void Start();
	void CleanUp();
	void Render(const float4x4& view, const float4x4& projection);

private:
	ShaderProgram* program = nullptr;

	uint vao = 0;
};

#endif // !__STATIC_GRID_RENDERER_H__