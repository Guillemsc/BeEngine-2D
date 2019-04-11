#ifndef __COMPONENT_SPRITE_RENDERER_H__
#define __COMPONENT_SPRITE_RENDERER_H__

#include "GameObjectComponent.h"
#include "ResourceSelectorWidget.h"
#include "GeometryMath.h"

class GameObject;
class ResourceTexture;
class Event;

class ComponentSpriteRenderer : public GameObjectComponent
{
	friend class GameObject;

private:
	void operator delete(void *) {}

public:
	ComponentSpriteRenderer();
	~ComponentSpriteRenderer();

	void EditorDraw();

	void Start();
	void Update();
	void CleanUp();

	void OnSaveAbstraction(DataAbstraction& abs);
	void OnLoadAbstraction(DataAbstraction& abs);

	void OnEvent(Event* ev);

	void OnChildAdded(GameObject* child);
	void OnChildRemoved(GameObject* child);
	void OnParentChanged(GameObject* new_parent);

	void SetResourceTexture(ResourceTexture* set);
	ResourceTexture* GetResourceTexture() const;

	void SetColour(const float4& set);
	float4 GetColour() const;

	void SetUseSprite(bool set);
	bool GetUseSprite() const;

	uint GetTextureId() const;
	float GetTextureHeightWidthRatio() const;
	float GetTextureWidthHeightRatio() const;
	float2 GetTextureSize() const;
	bool GetHasTexture() const;

	int GetLayer() const;

	void SetSize(const float2& size);
	float2 GetSize() const;

	void SetFilpX(bool set);
	bool GetFlipX() const;

	void SetFilpY(bool set);
	bool GetFlipY() const;

private:
	void SetComponentTransformSize();

private:
	ResourceTexture* resource_texture = nullptr;
	float4 colour = float4(1.0f, 0.0f, 1.0f, 1.0f);

	bool use_sprite = true;

	std::string resource_filter;

	bool flip_x = false;
	bool flip_y = false;

	float2 size = float2(20, 20);

	int layer = 0;

	float ui_scale_factor = 1.0f;

	ResourceSelectorWidget selector_widget;
};

#endif // !__COMPONENT_SPRITE_RENDERER_H__