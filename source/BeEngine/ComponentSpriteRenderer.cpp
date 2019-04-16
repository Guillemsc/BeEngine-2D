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
#include "ModuleGameObject.h"
#include "ComponentTransfrom.h"
#include "ScriptingBridgeComponentSpriteRenderer.h"
#include "ModuleAssets.h"
#include "ModuleTexture.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

ComponentSpriteRenderer::ComponentSpriteRenderer() 
	: GameObjectComponent(new ScriptingBridgeComponentSpriteRenderer(this),
	"Sprite Renderer", ComponentType::COMPONENT_TYPE_SPRITE_RENDERER,
	ComponentGroup::RENDERING, true)
{
}

ComponentSpriteRenderer::~ComponentSpriteRenderer()
{
}

void ComponentSpriteRenderer::EditorDraw()
{
	ImGui::DragInt("Layer", &layer, 1, 0, 9999);

	ImGui::Checkbox("Use sprite", &use_sprite);

	if (ImGui::DragFloat2("Size", (float*)&size, 1, 0))
	{
		SetSize(size);
	}

	if (use_sprite)
	{
		Resource* res = resource_texture;
		if (selector_widget.Draw("Sprite:", ResourceType::RESOURCE_TYPE_TEXTURE, res))
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

			ImGui::Spacing();
		}
	}
	else
	{
		float4 col = float4(colour.x, colour.y, colour.w, colour.z);
		if (ImGui::ColorEdit4("Colour", (float*)&col))
		{
			colour = float4(col.x, col.y, col.w, col.z);
		}
	}
}

void ComponentSpriteRenderer::Start()
{
	App->event->Suscribe(std::bind(&ComponentSpriteRenderer::OnEvent, this, std::placeholders::_1), EventType::RESOURCE_DESTROYED);

	if(GetOwner()->GetActive())
		App->scene_renderer->static_sprite_renderer->AddSpriteRenderer(this);
}

void ComponentSpriteRenderer::Update()
{
	SetComponentTransformSize();
}

void ComponentSpriteRenderer::CleanUp()
{
	App->scene_renderer->static_sprite_renderer->RemoveSpriteRenderer(this);

	App->event->UnSuscribe(std::bind(&ComponentSpriteRenderer::OnEvent, this, std::placeholders::_1), EventType::RESOURCE_DESTROYED);
}

void ComponentSpriteRenderer::OnSaveAbstraction(DataAbstraction & abs)
{
	abs.AddInt("layer", layer);

	abs.AddFloat2("size", size);

	if(resource_texture != nullptr)
		abs.AddString("resource", resource_texture->GetUID());

	abs.AddBool("use_sprite", use_sprite);
	abs.AddFloat4("colour", colour);

	abs.AddBool("flip_x", flip_x);
	abs.AddBool("flip_y", flip_y);
}

void ComponentSpriteRenderer::OnLoadAbstraction(DataAbstraction & abs)
{
	layer = abs.GetInt("layer");

	size = abs.GetFloat2("size", float2(10, 10));

	std::string resource_uid = abs.GetString("resource");

	if (resource_uid.compare("") != 0)
	{
		resource_texture = (ResourceTexture*)App->resource->GetResourceFromUid(resource_uid, ResourceType::RESOURCE_TYPE_TEXTURE);
	}

	use_sprite = abs.GetBool("use_sprite");
	colour = abs.GetFloat4("colour");

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

void ComponentSpriteRenderer::OnChangeActive(bool set)
{
	if (!set)
	{
		App->scene_renderer->static_sprite_renderer->RemoveSpriteRenderer(this);
	}
	else
	{
		App->scene_renderer->static_sprite_renderer->AddSpriteRenderer(this);
	}
}

void ComponentSpriteRenderer::SetResourceTexture(ResourceTexture* set)
{
	resource_texture = set;
}

ResourceTexture * ComponentSpriteRenderer::GetResourceTexture() const
{
	return resource_texture;
}

void ComponentSpriteRenderer::SetColour(const float4 & set)
{
	colour = set;
}

float4 ComponentSpriteRenderer::GetColour() const
{
	return colour;
}

void ComponentSpriteRenderer::SetUseSprite(bool set)
{
	use_sprite = set;
}

bool ComponentSpriteRenderer::GetUseSprite() const
{
	return use_sprite;
}

uint ComponentSpriteRenderer::GetTextureId() const
{
	uint ret = 0;

	if (resource_texture != nullptr)
	{
		ret = resource_texture->GetTextureId();
	}
	else
	{
		ret = App->assets->GetDefaultTexture()->GetId();
	}

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

int ComponentSpriteRenderer::GetLayer() const
{
	return layer;
}

void ComponentSpriteRenderer::SetSize(const float2 & set)
{
	size = set;

	if (size.x < 0)
		size.x = 0;

	if (size.y < 0)
		size.y = 0;
}

float2 ComponentSpriteRenderer::GetSize() const
{
	return size;
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

void ComponentSpriteRenderer::SetComponentTransformSize()
{	
	GetOwner()->transform->size = size;	
}
