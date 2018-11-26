#ifndef __TEXTURE_RENDERER_H__
#define __TEXTURE_RENDERER_H__

#include "Renderer.h"

#include "ModuleRenderer.h"
#include "ModuleShader.h"
#include "VertexBuffer.h"

class StaticTextureRenderer : public Renderer
{
public:
	StaticTextureRenderer();
	~StaticTextureRenderer();

	void Start();
	void CleanUp();
	void Render(const float4x4& view, const float4x4& projection);

private:
	ShaderProgram* program = nullptr;

	uint vao = 0;
	uint vbo = 0;
};

#endif // !__TEXTURE_RENDERER_H__