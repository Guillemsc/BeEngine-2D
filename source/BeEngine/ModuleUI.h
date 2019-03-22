#ifndef __MODULE_UI_H__
#define __MODULE_UI_H__

#include "Module.h"
#include "GeometryMath.h"

class ComponentButton;

class UIHandler
{
	friend class ModuleUI;

public:
	UIHandler();

	void SetTransfroms(const float2& pos, const float2& size, const float rotation_degrees);
	void SetLayer(int set);
	bool CheckRay(const LineSegment& ray);

	float2 GetPos() const;
	float2 GetSize() const;
	float GetRotationDegrees() const;

	bool GetHovered() const;
	bool GetPressed() const;

	void SetActive(bool set);
	bool GetActive() const;

private:
	float2 pos = float2::zero;
	float2 size = float2::zero;
	float rotation_degrees = 0.0f;
	OBB bbox;

	int layer = 0;

	bool hovered = false;
	bool pressed = false;

	bool active = true;	
};

class UIElement
{
	friend class ModuleUI;

public:
	UIElement();

	UIHandler* AddHandler();
	UIHandler* GetHandler(uint index) const;
	std::vector<UIHandler*> GetAllHandlers() const;

	void SetEnabled(bool set);

private:
	void DestroyAllHandlers();

private:
	std::vector<UIHandler*> handlers;

	bool enabled = true;
};

class ModuleUI : public Module
{
public:
	ModuleUI();
	~ModuleUI();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

	UIElement* CreateUIElement();
	void DestroyUIElement(UIElement* el);
	std::vector<UIElement*> GetAllElements() const;

private:
	void DestroyAllUIElements();

private:
	std::vector<UIElement*> elements;

};

#endif // !__MODULE_AUDIO_H__