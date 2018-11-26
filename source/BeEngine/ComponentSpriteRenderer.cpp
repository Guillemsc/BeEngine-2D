#include "ComponentSpriteRenderer.h"
#include "App.h"
#include "ResourceTexture.h"
#include "ModuleRenderer.h"
#include "StaticSpriteRenderer.h"

ComponentSpriteRenderer::ComponentSpriteRenderer() : GameObjectComponent("Sprite Renderer", ComponentType::SPRITE_RENDERER,
	ComponentGroup::RENDERING, true)
{
}

ComponentSpriteRenderer::~ComponentSpriteRenderer()
{
}

void ComponentSpriteRenderer::EditorDraw()
{
}

void ComponentSpriteRenderer::Start()
{
	App->renderer->static_sprite_renderer->AddSpriteRenderer(this);
}

void ComponentSpriteRenderer::CleanUp()
{
	App->renderer->static_sprite_renderer->RemoveSpriteRenderer(this);
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
