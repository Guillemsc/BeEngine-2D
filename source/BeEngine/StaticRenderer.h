#ifndef __STATIC_RENDERER_H__
#define __STATIC_RENDERER_H__

#include "Renderer.h"

#include "ModuleRenderer.h"
#include "StaticRendererItem.h"

class StaticSpriteRendererItem;
class SpriteRenderer;
class TextRenderer;
class ComponentSpriteRenderer;
class ComponentText;

class StaticRenderer : public Renderer
{
public:
	StaticRenderer();
	~StaticRenderer();

	void Start();
	void CleanUp();
	void Render(const float4x4& view, const float4x4& projection);

	StaticRendererItem* CreateRendererItem(ComponentSpriteRenderer* comp);
	StaticRendererItem* CreateRendererItem(ComponentText* comp);
	void UpdateRendererItem(StaticRendererItem* item);
	void DestroyRendererItem(StaticRendererItem* item);

public:
	SpriteRenderer* sprite_renderer = nullptr;
	TextRenderer* text_renderer = nullptr;

private:
	void AddRendererItem(StaticRendererItem* add);
	bool RemoveRendererItem(StaticRendererItem* remove);
	void UpdateRendererItemsToCheck();

private:
	std::map<int, std::vector<StaticRendererItem*>> renderer_items;

	std::vector<StaticRendererItem*> renderer_items_to_check;
};

#endif // !__STATIC_RENDERER_H__