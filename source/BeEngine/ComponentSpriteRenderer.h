#ifndef __COMPONENT_SPRITE_RENDERER_H__
#define __COMPONENT_SPRITE_RENDERER_H__

#include "GameObjectComponent.h"

#include "GeometryMath.h"

class GameObject;
class ResourceTexture;

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

	void OnChildAdded(GameObject* child);
	void OnChildRemoved(GameObject* child);
	void OnParentChanged(GameObject* new_parent);

	void SetResourceTexture(ResourceTexture* set);

private:
	ResourceTexture* resource_texture = nullptr;

};

#endif // !__COMPONENT_SPRITE_RENDERER_H__