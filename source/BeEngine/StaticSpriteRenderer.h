#ifndef __TEXTURE_RENDERER_H__
#define __TEXTURE_RENDERER_H__

#include "Renderer.h"

#include "ModuleRenderer.h"
#include "ModuleShader.h"
#include "VertexBuffer.h"
#include "ComponentSpriteRenderer.h"

class StaticSpriteRenderer : public Renderer
{
public:
	StaticSpriteRenderer();
	~StaticSpriteRenderer();

	void Start();
	void CleanUp();
	void Render(const float4x4& view, const float4x4& projection);

	void AddSpriteRenderer(ComponentSpriteRenderer* add);
	void RemoveSpriteRenderer(ComponentSpriteRenderer* remove);

private:
	ShaderProgram* program = nullptr;

	uint vao = 0;

	uint* quad_indices_buffer = nullptr;

	std::vector<ComponentSpriteRenderer*> sprite_renderers;
};

#endif // !__TEXTURE_RENDERER_H__