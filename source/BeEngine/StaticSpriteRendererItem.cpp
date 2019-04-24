#include "StaticSpriteRendererItem.h"
#include "ComponentSpriteRenderer.h"
#include "GameObject.h"
#include "StaticRenderer.h"
#include "SpriteRenderer.h"

StaticSpriteRendererItem::StaticSpriteRendererItem(ComponentSpriteRenderer* s)
{
	sprite = s;
}

StaticSpriteRendererItem::~StaticSpriteRendererItem()
{
}

void StaticSpriteRendererItem::Render(StaticRenderer* renderer, const float4x4 & view, const float4x4 & projection, float z_pos)
{
	renderer->sprite_renderer->Render(this, view, projection, z_pos);
}

int StaticSpriteRendererItem::GetCurrentLayer()
{
	return sprite->GetLayer();
}

int StaticSpriteRendererItem::GetCurrentHierarchyLayer()
{
	return sprite->GetOwner()->GetHierarcyLayer();
}

ComponentSpriteRenderer * StaticSpriteRendererItem::GetSpriteComponent() const
{
	return sprite;
}
