#ifndef __TEXT_RENDERER_H__
#define __TEXT_RENDERER_H__

#include "Renderer.h"

#include "ModuleRenderer.h"
#include "ModuleShader.h"
#include "VertexBuffer.h"

class StaticTextRendererItem;

class TextRenderer
{
public:
	TextRenderer();
	~TextRenderer();

	void Start();
	void CleanUp();
	void Render(StaticTextRendererItem* item, const float4x4& view, const float4x4& projection, int z_pos);

private:
	ShaderProgram* program = nullptr;

	uint vao = 0;
};

#endif // !__TEXT_RENDERER_H__