#include "ComponentButton.h"
#include "App.h"
#include "GameObject.h"
#include "ComponentTransfrom.h"
#include "ComponentSpriteRenderer.h"

ComponentButton::ComponentButton() : GameObjectComponent("Button", ComponentType::COMPONENT_TYPE_BUTTON, ComponentGroup::UI, true)
{
}

ComponentButton::~ComponentButton()
{
}

void ComponentButton::EditorDraw()
{
}

void ComponentButton::Start()
{
	ui_element = App->ui->CreateUIElement();
	handler = ui_element->AddHandler();
}

void ComponentButton::Update()
{
	if (c_sprite_renderer == nullptr)
	{

	}
	else
	{
		if (handler != nullptr)
		{
			if (handler->GetPressed())
			{

			}
			else if (handler->GetHovered())
			{

			}
			else
			{

			}
		}

		GameObject* owner = GetOwner();
		owner->transform->GetPosition();

		float2 size = c_sprite_renderer->GetTextureSize();

		size.x *= owner->transform->GetScale().x;
		size.y *= owner->transform->GetScale().y;

		handler->SetTransfroms(owner->transform->GetPosition(), size, owner->transform->GetRotationDegrees());
	}
}

void ComponentButton::CleanUp()
{
	handler = nullptr;
	App->ui->DestroyUIElement(ui_element);
	ui_element = nullptr;
}

void ComponentButton::OnSaveAbstraction(DataAbstraction & abs)
{
}

void ComponentButton::OnLoadAbstraction(DataAbstraction & abs)
{
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
