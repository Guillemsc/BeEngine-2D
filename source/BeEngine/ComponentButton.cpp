#include "ComponentButton.h"
#include "App.h"
#include "GameObject.h"
#include "ComponentTransfrom.h"
#include "ComponentSpriteRenderer.h"
#include "imgui.h"
#include "ModuleJson.h"

ComponentButton::ComponentButton() : GameObjectComponent("Button", ComponentType::COMPONENT_TYPE_BUTTON, ComponentGroup::UI, true)
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
	ui_element = App->ui->CreateUIElement();
	handler = ui_element->AddHandler();
}

void ComponentButton::Update()
{
	TryFindComponentSpriteRenderer();

	UpdateState();
}

void ComponentButton::CleanUp()
{
	handler = nullptr;
	App->ui->DestroyUIElement(ui_element);
	ui_element = nullptr;
}

void ComponentButton::OnSaveAbstraction(DataAbstraction & abs)
{
	abs.AddFloat4("colour_idle", colour_idle);
	abs.AddFloat4("colour_hover", colour_hover);
	abs.AddFloat4("colour_pressed", colour_pressed);
}

void ComponentButton::OnLoadAbstraction(DataAbstraction & abs)
{
	colour_idle = abs.GetFloat4("colour_idle");
	colour_hover = abs.GetFloat4("colour_hover");
	colour_pressed = abs.GetFloat4("colour_pressed");
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
			}
			else if (handler->GetHovered())
			{
				c_sprite_renderer->SetColour(colour_hover);
			}
			else
			{
				c_sprite_renderer->SetColour(colour_idle);
			}
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
