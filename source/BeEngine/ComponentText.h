#ifndef __COMPONENT_TEXT_H__
#define __COMPONENT_TEXT_H__

#include "GameObjectComponent.h"
#include "GeometryMath.h"
#include "ResourceSelectorWidget.h"
#include "ModuleText.h"

class GameObject;
class Event;
class ResourceFont;

class ComponentText : public GameObjectComponent
{
	friend class GameObject;

private:
	void operator delete(void *) {}

public:
	ComponentText();
	~ComponentText();

	void EditorDraw();

	void Start();
	void Update();
	void CleanUp();

	void OnSaveAbstraction(DataAbstraction& abs);
	void OnLoadAbstraction(DataAbstraction& abs);

	void OnEvent(Event* ev);

	void OnChildAdded(GameObject* child);
	void OnChildRemoved(GameObject* child);
	void OnParentChanged(GameObject* new_parent);

	void RenderGuizmos(float relative_size);

	void SetText(const std::string& text);
	void SetTextSize(int size);

	TextData GetTextData() const;
	Font* GetCurrentFont() const;
	float4 GetColour() const;
	int GetLayer() const;

	bool GetRenderQuads() const;

private:
	void UpdateFont();
	void UpdateTextGlyphs();

private:
	ResourceFont* resource_font = nullptr;

	Font* font = nullptr;

	std::string drawing_text = "Text";
	int text_size = 12;
	float4 colour = float4(1.0f, 1.0f, 1.0f, 1.0f);

	TextData text_data;

	bool render_quads = false;

	int layer = 0;

	ResourceSelectorWidget selector_widget;
};

#endif // !__COMPONENT_TEXT_H__