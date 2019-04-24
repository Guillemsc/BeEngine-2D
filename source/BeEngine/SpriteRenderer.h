#ifndef __SPRITE_RENDERER_H__
#define __SPRITE_RENDERER_H__

#include "Renderer.h"

#include "ModuleRenderer.h"
#include "ModuleShader.h"
#include "VertexBuffer.h"
#include "ComponentSpriteRenderer.h"

class StaticSpriteRendererItem;

class SpriteRenderer 
{
public:
	void Start();
	void CleanUp();
	void Render(StaticSpriteRendererItem* item, const float4x4& view, const float4x4& projection, int z_pos);

private:
	ShaderProgram* program = nullptr;

	uint vao = 0;
};

#endif // !__SPRITE_RENDERER_H__