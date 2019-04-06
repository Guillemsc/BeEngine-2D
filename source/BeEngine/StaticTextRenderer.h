#ifndef __STATIC_TEXT_RENDERER_H__
#define __STATIC_TEXT_RENDERER_H__

#include "Renderer.h"

#include "ModuleRenderer.h"
#include "ModuleShader.h"
#include "VertexBuffer.h"
#include "ComponentSpriteRenderer.h"

class ComponentText;

class StaticTextRenderer : public Renderer
{
public:
	StaticTextRenderer();
	~StaticTextRenderer();

	void Start();
	void CleanUp();
	void Render(const float4x4& view, const float4x4& projection);

	void AddTextRenderer(ComponentText* add);
	void RemoveTextRenderer(ComponentText* remove);

private:
	ShaderProgram* program = nullptr;

	uint vao = 0;

	std::vector<ComponentText*> text_renderers;
};

#endif // !__STATIC_TEXT_RENDERER_H__