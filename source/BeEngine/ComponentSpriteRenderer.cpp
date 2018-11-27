#include "ComponentSpriteRenderer.h"
#include "App.h"
#include "ResourceTexture.h"
#include "ModuleRenderer.h"
#include "StaticSpriteRenderer.h"
#include "App.h"
#include "ModuleResource.h"
#include "imgui.h"

ComponentSpriteRenderer::ComponentSpriteRenderer() : GameObjectComponent("Sprite Renderer", ComponentType::SPRITE_RENDERER,
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
	if (App->resource->EditorResourceSelector(ResourceType::TEXTURE, res, resource_filter))
	{
		resource_texture = (ResourceTexture*)res;

		App->renderer->static_sprite_renderer->AddSpriteRenderer(this);
	}

	if (resource_texture != nullptr)
	{
		float ratio = resource_texture->GetHeightWidthRatio();
		ImGui::Image((ImTextureID)resource_texture->GetTextureId(), ImVec2(100, 100 * ratio));
	}
}

void ComponentSpriteRenderer::Start()
{

}

void ComponentSpriteRenderer::CleanUp()
{
	//App->renderer->static_sprite_renderer->RemoveSpriteRenderer(this);

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
