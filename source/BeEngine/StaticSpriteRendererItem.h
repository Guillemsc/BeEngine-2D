#ifndef __STATIC_SPRITE_RENDERER_ITEM_H__
#define __STATIC_SPRITE_RENDERER_ITEM_H__

#include "StaticRendererItem.h"

#include "ModuleRenderer.h"
#include "ModuleShader.h"

class ComponentSpriteRenderer;
class StaticRenderer;

class StaticSpriteRendererItem : public StaticRendererItem
{
public:
	StaticSpriteRendererItem(ComponentSpriteRenderer* sprite);
	~StaticSpriteRendererItem();

	void Render(StaticRenderer* renderer, const float4x4& view, const float4x4 & projection, float z_pos);

	int GetCurrentLayer();
	int GetCurrentHierarchyLayer();

	ComponentSpriteRenderer* GetSpriteComponent() const;

private:
	ComponentSpriteRenderer* sprite = nullptr;
};

#endif // !__STATIC_SPRITE_RENDERER_ITEM_H__