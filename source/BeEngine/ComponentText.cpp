#include "ComponentText.h"
#include "App.h"
#include "ResourceFont.h"
#include "ScriptingBridgeComponentText.h"
#include "ResourceFont.h"
#include "ModuleEvent.h"
#include "ModuleSceneRenderer.h"
#include "StaticTextRenderer.h"

ComponentText::ComponentText() : GameObjectComponent(new ScriptingBridgeComponentText(this),
	"Text", ComponentType::COMPONENT_TYPE_TEXT, ComponentGroup::UI, true)
{
}

ComponentText::~ComponentText()
{
}

void ComponentText::EditorDraw()
{
	ImGui::Checkbox("Render quads", &render_quads);

	char name_arr[999];
	strcpy_s(name_arr, 999, drawing_text.c_str());
	if (ImGui::InputText("Showing text", name_arr, 998, ImGuiInputTextFlags_AutoSelectAll))
		SetText(name_arr);

	Resource* res = resource_font;
	if (selector_widget.Draw("Font: ", ResourceType::RESOURCE_TYPE_FONT, res))
	{
		resource_font = (ResourceFont*)res;

		UpdateFont();
	}

	if (ImGui::DragInt("Text resolution", &text_size))
	{
		SetTextSize(text_size);
	}

	float4 col = float4(colour.x, colour.y, colour.w, colour.z);
	if (ImGui::ColorEdit4("Colour", (float*)&col))
	{
		colour = float4(col.x, col.y, col.w, col.z);
	}
}

void ComponentText::Start()
{
	InitBeObject();

	SetTextSize(40);

	App->scene_renderer->static_text_renderer->AddTextRenderer(this);

	App->event->Suscribe(std::bind(&ComponentText::OnEvent, this, std::placeholders::_1), EventType::RESOURCE_DESTROYED);
}

void ComponentText::Update()
{
}

void ComponentText::CleanUp()
{
	App->scene_renderer->static_text_renderer->RemoveTextRenderer(this);

	App->event->UnSuscribe(std::bind(&ComponentText::OnEvent, this, std::placeholders::_1), EventType::RESOURCE_DESTROYED);

	CleanUpBeObject();
}

void ComponentText::OnSaveAbstraction(DataAbstraction & abs)
{
}

void ComponentText::OnLoadAbstraction(DataAbstraction & abs)
{
}

void ComponentText::OnEvent(Event * ev)
{
	if (ev->GetType() == EventType::RESOURCE_DESTROYED)
	{
		EventResourceDestroyed* erd = (EventResourceDestroyed*)ev;

		if (erd->GetResource() == resource_font)
		{
			resource_font = nullptr;

			UpdateFont();
		}
	}
}

void ComponentText::OnChildAdded(GameObject * child)
{
}

void ComponentText::OnChildRemoved(GameObject * child)
{
}

void ComponentText::OnParentChanged(GameObject * new_parent)
{
}

void ComponentText::RenderGuizmos(float relative_size)
{
}

void ComponentText::SetText(const std::string & text)
{
	if (text.compare(drawing_text) != 0)
	{
		drawing_text = text;

		UpdateTextGlyphs();
	}
}

void ComponentText::SetTextSize(int size)
{
	text_size = size;

	if (text_size < 3)
		text_size = 3;

	UpdateFont();
}

std::vector<Glyph> ComponentText::GetTextData() const
{
	return text_glyphs;
}

Font * ComponentText::GetCurrentFont() const
{
	return font;
}

float4 ComponentText::GetColour() const
{
	return colour;
}

bool ComponentText::GetRenderQuads() const
{
	return render_quads;
}

void ComponentText::UpdateFont()
{
	if (font != nullptr)
	{
		App->text->DestroyFont(font);
		font = nullptr;
	}

	if (resource_font != nullptr)
	{
		if (font == nullptr)
		{
			font = App->text->GenerateFont(resource_font->GetFace(), text_size);
		}
	}

	UpdateTextGlyphs();
}

void ComponentText::UpdateTextGlyphs()
{
	text_glyphs.clear();

	if (font != nullptr)
	{
		text_glyphs = font->GetTextGlyphs(drawing_text);
	}
}
