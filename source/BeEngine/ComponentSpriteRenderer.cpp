#include "ComponentSpriteRenderer.h"
#include "App.h"
#include "ResourceTexture.h"
#include "ModuleRenderer.h"
#include "StaticSpriteRenderer.h"
#include "ModuleResource.h"
#include "imgui.h"
#include "ModuleEvent.h"
#include "ModuleSceneRenderer.h"
#include "ModuleJson.h"

ComponentSpriteRenderer::ComponentSpriteRenderer() : GameObjectComponent("Sprite Renderer", ComponentType::COMPONENT_TYPE_SPRITE_RENDERER,
	ComponentGroup::RENDERING, true)
{
}

ComponentSpriteRenderer::~ComponentSpriteRenderer()
{
}

void ComponentSpriteRenderer::EditorDraw()
{
	ImGui::Text("Sprite:   ");

	ImGui::SameLine();

	Resource* res = resource_texture;
	if (App->resource->EditorResourceSelector(ResourceType::RESOURCE_TYPE_TEXTURE, res, resource_filter))
	{
		resource_texture = (ResourceTexture*)res;
	}

	if (resource_texture != nullptr)
	{
		ImGui::Text("Filp");
		ImGui::SameLine();
		ImGui::Checkbox("X", &flip_x);
		ImGui::SameLine();
		ImGui::Checkbox("Y", &flip_y);

		ImGui::Spacing();

		float2 image_size = resource_texture->GetSize();

		std::string size_text = "Size: x: " + std::to_string((int)image_size.x) + "  y: " + std::to_string((int)image_size.y);
		ImGui::Text(size_text.c_str());

		float ratio = resource_texture->GetHeightWidthRatio();
		ImGui::Image((ImTextureID)resource_texture->GetTextureId(), ImVec2(100, 100 * ratio));
	}
}

void ComponentSpriteRenderer::Start()
{
	App->event->Suscribe(std::bind(&ComponentSpriteRenderer::OnEvent, this, std::placeholders::_1), EventType::RESOURCE_DESTROYED);

	App->scene_renderer->static_sprite_renderer->AddSpriteRenderer(this);
}

void ComponentSpriteRenderer::CleanUp()
{
	App->scene_renderer->static_sprite_renderer->RemoveSpriteRenderer(this);

	App->event->UnSuscribe(std::bind(&ComponentSpriteRenderer::OnEvent, this, std::placeholders::_1), EventType::RESOURCE_DESTROYED);
}

void ComponentSpriteRenderer::OnSaveAbstraction(DataAbstraction & abs)
{
	if(resource_texture != nullptr)
		abs.AddString("resource", resource_texture->GetUID());

	abs.AddBool("flip_x", flip_x);
	abs.AddBool("flip_y", flip_y);
}

void ComponentSpriteRenderer::OnLoadAbstraction(DataAbstraction & abs)
{
	std::string resource_uid = abs.GetString("resource");

	if (resource_uid.compare("") != 0)
	{
		resource_texture = (ResourceTexture*)App->resource->GetResourceFromUid(resource_uid, ResourceType::RESOURCE_TYPE_TEXTURE);
	}

	flip_x = abs.GetBool("flip_x");
	flip_y = abs.GetBool("flip_y");
}

void ComponentSpriteRenderer::OnEvent(Event * ev)
{
	if (ev->GetType() == EventType::RESOURCE_DESTROYED)
	{
		EventResourceDestroyed* erd = (EventResourceDestroyed*)ev;

		if (erd->GetResource() == resource_texture)
			resource_texture = nullptr;
	}
}

void ComponentSpriteRenderer::OnChildAdded(GameObject * child)
{
}

void ComponentSpriteRenderer::OnChildRemoved(GameObject * child)
{
}

void ComponentSpriteRenderer::OnParentChanged(GameObject * new_parent)
{
}

void ComponentSpriteRenderer::SetResourceTexture(ResourceTexture* set)
{
	resource_texture = set;
}

uint ComponentSpriteRenderer::GetTextureId() const
{
	uint ret = 0;

	if (resource_texture != nullptr)
		ret = resource_texture->GetTextureId();

	return ret;
}

float ComponentSpriteRenderer::GetTextureHeightWidthRatio() const
{
	float ret = 1.0f;

	if (resource_texture != nullptr)
		ret = resource_texture->GetHeightWidthRatio();

	return ret;
}

float ComponentSpriteRenderer::GetTextureWidthHeightRatio() const
{
	float ret = 1.0f;

	if (resource_texture != nullptr)
		ret = resource_texture->GetWidthHeightRatio();

	return ret;
}

float2 ComponentSpriteRenderer::GetTextureSize() const
{
	float2 ret = float2::zero;

	if (resource_texture != nullptr)
		ret = resource_texture->GetSize();

	return ret;
}

bool ComponentSpriteRenderer::GetHasTexture() const
{
	return resource_texture != nullptr;
}

void ComponentSpriteRenderer::SetFilpX(bool set)
{
	flip_x = set;
}

bool ComponentSpriteRenderer::GetFlipX() const
{
	return flip_x;
}

void ComponentSpriteRenderer::SetFilpY(bool set)
{
	flip_y = set;
}

bool ComponentSpriteRenderer::GetFlipY() const
{
	return flip_y;
}
