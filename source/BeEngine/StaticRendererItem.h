#ifndef __STATIC_RENDERER_ITEM_H__
#define __STATIC_RENDERER_ITEM_H__

#include "Renderer.h"

#include "ModuleRenderer.h"
#include "ModuleShader.h"

class StaticRenderer;

class StaticRendererItem 
{
	friend class StaticRenderer;

private:
	void operator delete(void *) {}

public:
	StaticRendererItem();
	~StaticRendererItem();

	virtual void Render(StaticRenderer* renderer, const float4x4& view, const float4x4 & projection, float z_pos) {};

	virtual int GetCurrentLayer() { return 0; };
	virtual int GetCurrentHierarchyLayer() { return 0; };

protected:
	int last_layer = 0;
	int last_hierachy_layer = 0;
};

#endif // !__STATIC_RENDERER_ITEM_H__