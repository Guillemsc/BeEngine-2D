#ifndef __COMPONENT_BUTTON_H__
#define __COMPONENT_BUTTON_H__

#include "GameObjectComponent.h"
#include "ModuleUI.h"

#include "GeometryMath.h"

class GameObject;
class Event;
class ResourceTexture;
class Event;
class ScriptingBridgeComponentButton;

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
	void OnEvent(Event * ev);

	void TryFindComponentSpriteRenderer();

	void UpdateState();

private:
	UIElement* ui_element = nullptr;
	UIHandler* handler = nullptr;

	ComponentSpriteRenderer* c_sprite_renderer = nullptr;

	bool use_sprites = false;

	ResourceTexture* resource_texture_idle = nullptr;
	ResourceTexture* resource_texture_hover = nullptr;
	ResourceTexture* resource_texture_pressed = nullptr;

	float4 colour_idle = float4(1.0f, 0.0f, 1.0f, 1.0f);
	float4 colour_hover = float4(0.7f, 0.0f, 1.0f, 1.0f);
	float4 colour_pressed = float4(1.0f, 0.0f, 1.0f, 0.0f);

	std::string resource_filter;

	bool last_frame_pressed = false;

private:
	ScriptingBridgeComponentButton* scripting_bridge = nullptr;
};

#endif // !__COMPONENT_BUTTON_H__