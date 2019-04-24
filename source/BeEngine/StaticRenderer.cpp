#include "StaticRenderer.h"
#include "App.h"
#include "ModuleSceneRenderer.h"
#include "SpriteRenderer.h"
#include "StaticSpriteRendererItem.h"
#include "TextRenderer.h"
#include "StaticTextRendererItem.h"

StaticRenderer::StaticRenderer()
{
}

StaticRenderer::~StaticRenderer()
{
}

void StaticRenderer::Start()
{
	sprite_renderer = new SpriteRenderer();
	sprite_renderer->Start();

	text_renderer = new TextRenderer();
	text_renderer->Start();
}

void StaticRenderer::CleanUp()
{
	sprite_renderer->CleanUp();
	RELEASE(sprite_renderer);

	text_renderer->CleanUp();
	RELEASE(text_renderer);
}

void StaticRenderer::Render(const float4x4 & view, const float4x4 & projection)
{
	UpdateRendererItemsToCheck();

	int curr_layer = 0;
	for (std::map<int, std::vector<StaticRendererItem*>>::iterator it = renderer_items.begin(); it != renderer_items.end(); ++it)
	{
		std::vector<StaticRendererItem*> items = (*it).second;

		for (std::vector<StaticRendererItem*>::iterator s = items.begin(); s != items.end(); ++s, ++curr_layer)
		{
			// Z pos
			float z_pos = App->scene_renderer->layer_space_component_scene.GetLayerValue(curr_layer);

			// Render
			(*s)->Render(this, view, projection, z_pos);

			// Update Sprites renderer layer
			if ((*s)->last_layer != (*s)->GetCurrentLayer() || (*s)->last_hierachy_layer != (*s)->GetCurrentHierarchyLayer())
			{
				renderer_items_to_check.push_back((*s));
			}
		}
	}
}

StaticRendererItem * StaticRenderer::CreateRendererItem(ComponentSpriteRenderer * comp)
{
	StaticRendererItem* ret = nullptr;

	if (comp != nullptr)
	{
		ret = new StaticSpriteRendererItem(comp);

		AddRendererItem(ret);
	}

	return ret;
}

StaticRendererItem * StaticRenderer::CreateRendererItem(ComponentText * comp)
{
	StaticRendererItem* ret = nullptr;

	if (comp != nullptr)
	{
		ret = new StaticTextRendererItem(comp);

		AddRendererItem(ret);
	}

	return ret;
}

void StaticRenderer::DestroyRendererItem(StaticRendererItem * item)
{
	if (item != nullptr)
	{
		RemoveRendererItem(item);

		RELEASE(item);
	}
}

void StaticRenderer::AddRendererItem(StaticRendererItem * add)
{
	if (add != nullptr)
	{
		std::map<int, std::vector<StaticRendererItem*>>::iterator it = renderer_items.find(add->GetCurrentLayer());

		if (it != renderer_items.end())
		{
			add->last_layer = add->GetCurrentLayer();
			add->last_hierachy_layer = add->GetCurrentHierarchyLayer();

			bool added = false;
			int counter = 0;
			for (std::vector<StaticRendererItem*>::iterator ite = it->second.begin(); ite != it->second.end(); ++ite, ++counter)
			{
				if ((*ite)->GetCurrentHierarchyLayer() > add->last_hierachy_layer)
				{
					it->second.insert(ite, add);
					added = true;
					break;
				}
			}

			if (!added)
				it->second.push_back(add);
		}
		else
		{
			add->last_layer = add->GetCurrentLayer();
			add->last_hierachy_layer = add->GetCurrentHierarchyLayer();

			std::vector<StaticRendererItem*> items;
			items.push_back(add);

			renderer_items[add->GetCurrentLayer()] = items;
		}
	}
}

bool StaticRenderer::RemoveRendererItem(StaticRendererItem * remove)
{
	bool ret = false;

	if (remove != nullptr)
	{
		for (std::map<int, std::vector<StaticRendererItem*>>::iterator it = renderer_items.begin(); it != renderer_items.end(); ++it)
		{
			for (std::vector<StaticRendererItem*>::iterator s = (*it).second.begin(); s != (*it).second.end(); ++s)
			{
				if ((*s) == remove)
				{
					(*it).second.erase(s);

					ret = true;
					break;
				}
			}

			if (ret)
				break;
		}

		for (std::vector<StaticRendererItem*>::iterator it = renderer_items_to_check.begin(); it != renderer_items_to_check.end(); ++it)
		{
			if ((*it) == remove)
			{
				renderer_items_to_check.erase(it);
				break;
			}
		}
	}

	return ret;
}

void StaticRenderer::UpdateRendererItemsToCheck()
{
	while (renderer_items_to_check.size() > 0)
	{
		StaticRendererItem* curr = *renderer_items_to_check.begin();

		renderer_items_to_check.erase(renderer_items_to_check.begin());

		RemoveRendererItem(curr);

		AddRendererItem(curr);
	}
}
