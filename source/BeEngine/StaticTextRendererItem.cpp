#include "StaticTextRendererItem.h"
#include "ComponentText.h"
#include "GameObject.h"
#include "StaticRenderer.h"
#include "TextRenderer.h"

StaticTextRendererItem::StaticTextRendererItem(ComponentText * t)
{
	text = t;
}

StaticTextRendererItem::~StaticTextRendererItem()
{
}

void StaticTextRendererItem::Render(StaticRenderer * renderer, const float4x4 & view, const float4x4 & projection, float z_pos)
{
	renderer->text_renderer->Render(this, view, projection, z_pos);
}

int StaticTextRendererItem::GetCurrentLayer()
{
	return text->GetLayer();
}

int StaticTextRendererItem::GetCurrentHierarchyLayer()
{
	return text->GetOwner()->GetHierarcyLayer();
}

ComponentText * StaticTextRendererItem::GetTextComponent() const
{
	return text;
}
