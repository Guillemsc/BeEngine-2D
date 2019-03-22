#ifndef __COMPONENT_BUTTON_H__
#define __COMPONENT_BUTTON_H__

#include "GameObjectComponent.h"
#include "ModuleUI.h"

#include "GeometryMath.h"

class GameObject;
class Event;

class ComponentSpriteRenderer;

class ComponentButton : public GameObjectComponent
{
	friend class GameObject;

private:
	void operator delete(void *) {}

public:
	ComponentButton();
	~ComponentButton();

	void EditorDraw();

	void Start();
	void Update();
	void CleanUp();

	void OnSaveAbstraction(DataAbstraction& abs);
	void OnLoadAbstraction(DataAbstraction& abs);

	void OnChildAdded(GameObject* child);
	void OnChildRemoved(GameObject* child);
	void OnParentChanged(GameObject* new_parent);

	void OnAddComponent(GameObjectComponent* new_component);
	void OnRemoveComponent(GameObjectComponent* new_component);

	void RenderGuizmos(float relative_size);

private:
	UIElement* ui_element = nullptr;
	UIHandler* handler = nullptr;

	ComponentSpriteRenderer* c_sprite_renderer = nullptr;



};

#endif // !__COMPONENT_BUTTON_H__