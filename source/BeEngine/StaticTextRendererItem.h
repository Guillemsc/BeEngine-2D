#ifndef __STATIC_TEXT_RENDERER_ITEM_H__
#define __STATIC_TEXT_RENDERER_ITEM_H__

#include "StaticRendererItem.h"

#include "ModuleRenderer.h"
#include "ModuleShader.h"

class ComponentText;
class StaticRenderer;

class StaticTextRendererItem : public StaticRendererItem
{
public:
	StaticTextRendererItem(ComponentText* text);
	~StaticTextRendererItem();

	void Render(StaticRenderer* renderer, const float4x4& view, const float4x4 & projection, float z_pos);

	int GetCurrentLayer();
	int GetCurrentHierarchyLayer();

	ComponentText* GetTextComponent() const;

private:
	ComponentText* text = nullptr;
};

#endif // !__STATIC_TEXT_RENDERER_ITEM_H__