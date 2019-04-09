#include "ComponentButton.h"
#include "App.h"
#include "GameObject.h"
#include "ComponentTransfrom.h"
#include "ComponentSpriteRenderer.h"
#include "imgui.h"
#include "ModuleJson.h"
#include "Resource.h"
#include "ModuleResource.h"
#include "ResourceTexture.h"
#include "Event.h"
#include "ModuleEvent.h"
#include "ScriptingBridgeComponentButton.h"

ComponentButton::ComponentButton() 
	: GameObjectComponent(new ScriptingBridgeComponentButton(this),
		"Button", ComponentType::COMPONENT_TYPE_BUTTON, ComponentGroup::UI, true)
{
}

ComponentButton::~ComponentButton()
{
}

void ComponentButton::EditorDraw()
{
	if (c_sprite_renderer != nullptr)
	{
		ImGui::Checkbox("Use sprites", &use_sprites);

		if (use_sprites)
		{
			Resource* res_1 = (Resource*)resource_texture_idle;
			if (selector_widget_1.Draw("Idle:", ResourceType::RESOURCE_TYPE_TEXTURE, res_1))
			{
				resource_texture_idle = (ResourceTexture*)res_1;
			}

			Resource* res_2 = (Resource*)resource_texture_hover;
			if (selector_widget_2.Draw("Hover:", ResourceType::RESOURCE_TYPE_TEXTURE, res_2))
			{
				resource_texture_hover = (ResourceTexture*)res_2;
			}

			Resource* res_3 = (Resource*)resource_texture_pressed;
			if (selector_widget_2.Draw("Pressed:", ResourceType::RESOURCE_TYPE_TEXTURE, res_3))
			{
				resource_texture_pressed = (ResourceTexture*)res_3;
			}
		}
		else
		{
			float4 col_idle = float4(colour_idle.x, colour_idle.y, colour_idle.w, colour_idle.z);
			if (ImGui::ColorEdit4("Idle", (float*)&col_idle))
				colour_idle = float4(col_idle.x, col_idle.y, col_idle.w, col_idle.z);

			float4 col_hover = float4(colour_hover.x, colour_hover.y, colour_hover.w, colour_hover.z);
			if (ImGui::ColorEdit4("Hover", (float*)&col_hover))
				colour_hover = float4(col_hover.x, col_hover.y, col_hover.w, col_hover.z);

			float4 col_pressed = float4(colour_pressed.x, colour_pressed.y, colour_pressed.w, colour_pressed.z);
			if (ImGui::ColorEdit4("Pressed", (float*)&col_pressed))
				colour_pressed = float4(col_pressed.x, col_pressed.y, col_pressed.w, col_pressed.z);
		}
	}
	else
	{
		ImGui::Text("Button needs a component Sprite Renderer");
		if (ImGui::Button("Create component Sprite Renderer"))
			GetOwner()->CreateComponent(ComponentType::COMPONENT_TYPE_SPRITE_RENDERER);
	}
}

void ComponentButton::Start()
{
	App->event->Suscribe(std::bind(&ComponentButton::OnEvent, this, std::placeholders::_1), EventType::RESOURCE_DESTROYED);

	ui_element = App->ui->CreateUIElement();
	handler = ui_element->AddHandler();

	TryFindComponentSpriteRenderer();
}

void ComponentButton::Update()
{
	UpdateState();
}

void ComponentButton::CleanUp()
{
	handler = nullptr;
	App->ui->DestroyUIElement(ui_element);
	ui_element = nullptr;

	App->event->UnSuscribe(std::bind(&ComponentButton::OnEvent, this, std::placeholders::_1), EventType::RESOURCE_DESTROYED);
}

void ComponentButton::OnSaveAbstraction(DataAbstraction & abs)
{
	abs.AddBool("use_sprites", use_sprites);

	abs.AddFloat4("colour_idle", colour_idle);
	abs.AddFloat4("colour_hover", colour_hover);
	abs.AddFloat4("colour_pressed", colour_pressed);

	if (resource_texture_idle != nullptr)
		abs.AddString("resource_idle", resource_texture_idle->GetUID());

	if (resource_texture_hover != nullptr)
		abs.AddString("resource_hover", resource_texture_hover->GetUID());

	if (resource_texture_pressed != nullptr)
		abs.AddString("resource_pressed", resource_texture_pressed->GetUID());
}

void ComponentButton::OnLoadAbstraction(DataAbstraction & abs)
{
	use_sprites = abs.GetBool("use_sprites");

	colour_idle = abs.GetFloat4("colour_idle");
	colour_hover = abs.GetFloat4("colour_hover");
	colour_pressed = abs.GetFloat4("colour_pressed");

	std::string resource_idle_uid = abs.GetString("resource_idle");

	if (resource_idle_uid.compare("") != 0)
		resource_texture_idle = (ResourceTexture*)App->resource->GetResourceFromUid(resource_idle_uid, ResourceType::RESOURCE_TYPE_TEXTURE);

	std::string resource_hover_uid = abs.GetString("resource_hover");

	if (resource_hover_uid.compare("") != 0)
		resource_texture_hover = (ResourceTexture*)App->resource->GetResourceFromUid(resource_hover_uid, ResourceType::RESOURCE_TYPE_TEXTURE);

	std::string resource_pressed_uid = abs.GetString("resource_pressed");

	if (resource_pressed_uid.compare("") != 0)
		resource_texture_pressed = (ResourceTexture*)App->resource->GetResourceFromUid(resource_pressed_uid, ResourceType::RESOURCE_TYPE_TEXTURE);
	
}

void ComponentButton::OnChildAdded(GameObject * child)
{
}

void ComponentButton::OnChildRemoved(GameObject * child)
{
}

void ComponentButton::OnParentChanged(GameObject * new_parent)
{
}

void ComponentButton::OnAddComponent(GameObjectComponent * new_component)
{
	if (new_component->GetType() == ComponentType::COMPONENT_TYPE_SPRITE_RENDERER)
	{
		c_sprite_renderer = (ComponentSpriteRenderer*)new_component;
	}
}

void ComponentButton::OnRemoveComponent(GameObjectComponent * new_component)
{
	if (new_component->GetType() == ComponentType::COMPONENT_TYPE_SPRITE_RENDERER)
	{
		if (c_sprite_renderer == (ComponentSpriteRenderer*)new_component)
			c_sprite_renderer = nullptr;
	}
}

void ComponentButton::RenderGuizmos(float relative_size)
{
}

void ComponentButton::OnEvent(Event * ev)
{	
	if (ev->GetType() == EventType::RESOURCE_DESTROYED)
	{
		EventResourceDestroyed* erd = (EventResourceDestroyed*)ev;

		if (erd->GetResource() == resource_texture_pressed)
			resource_texture_pressed = nullptr;

		if (erd->GetResource() == resource_texture_hover)
			resource_texture_hover = nullptr;

		if (erd->GetResource() == resource_texture_idle)
			resource_texture_idle = nullptr;
	}
}

void ComponentButton::TryFindComponentSpriteRenderer()
{
	if (c_sprite_renderer == nullptr)
		c_sprite_renderer = (ComponentSpriteRenderer*)GetOwner()->GetComponent(ComponentType::COMPONENT_TYPE_SPRITE_RENDERER);
}

void ComponentButton::UpdateState()
{
	if (c_sprite_renderer == nullptr)
	{

	}
	else
	{
		c_sprite_renderer->SetUseSprite(use_sprites);

		if (handler != nullptr)
		{
			if (handler->GetPressed())
			{
				c_sprite_renderer->SetColour(colour_pressed);
				c_sprite_renderer->SetResourceTexture(resource_texture_pressed);
			}
			else if (handler->GetHovered())
			{
				c_sprite_renderer->SetColour(colour_hover);
				c_sprite_renderer->SetResourceTexture(resource_texture_hover);
			}
			else
			{
				c_sprite_renderer->SetColour(colour_idle);
				c_sprite_renderer->SetResourceTexture(resource_texture_idle);
			}

			if (handler->GetPressed())
			{
				if (!last_frame_pressed)
				{
					ScriptingBridgeComponentButton* bridge = (ScriptingBridgeComponentButton*)GetScriptingBridge();
					bridge->CallOnClick();
				}

				last_frame_pressed = true;
			} 
			else
				last_frame_pressed = false;
		}

		GameObject* owner = GetOwner();
		owner->transform->GetPosition();

		float2 size = float2::zero;

		size.x = owner->transform->GetScale().x * owner->transform->GetSize().x;
		size.y = owner->transform->GetScale().y * owner->transform->GetSize().y;

		handler->SetTransfroms(owner->transform->GetPosition(), size, owner->transform->GetRotationDegrees());
		handler->SetLayer(c_sprite_renderer->GetLayer());
	}
}
