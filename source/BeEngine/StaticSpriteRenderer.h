#ifndef __STATIC_SPRITE_RENDERER_H__
#define __STATIC_SPRITE_RENDERER_H__

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

	std::vector<ComponentSpriteRenderer*> sprite_renderers;
};

#endif // !__STATIC_SPRITE_RENDERER_H__