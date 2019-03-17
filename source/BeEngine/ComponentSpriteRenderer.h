#ifndef __COMPONENT_SPRITE_RENDERER_H__
#define __COMPONENT_SPRITE_RENDERER_H__

#include "GameObjectComponent.h"

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
	void CleanUp();

	void OnSaveAbstraction(DataAbstraction& abs);
	void OnLoadAbstraction(DataAbstraction& abs);

	void OnEvent(Event* ev);

	void OnChildAdded(GameObject* child);
	void OnChildRemoved(GameObject* child);
	void OnParentChanged(GameObject* new_parent);

	void SetResourceTexture(ResourceTexture* set);

	uint GetTextureId() const;
	float GetTextureHeightWidthRatio() const;
	float GetTextureWidthHeightRatio() const;
	float2 GetTextureSize() const;
	bool GetHasTexture() const;

	int GetLayer() const;

	void SetFilpX(bool set);
	bool GetFlipX() const;

	void SetFilpY(bool set);
	bool GetFlipY() const;

private:
	ResourceTexture* resource_texture = nullptr;

	std::string resource_filter;

	bool flip_x = false;
	bool flip_y = false;

	int layer = 0;

	float ui_scale_factor = 1.0f;
};

#endif // !__COMPONENT_SPRITE_RENDERER_H__