#include "ModuleUI.h"

ModuleUI::ModuleUI()
{
}

ModuleUI::~ModuleUI()
{
}

bool ModuleUI::Awake()
{
	bool ret = true;

	return ret;
}

bool ModuleUI::Start()
{
	bool ret = true;

	return ret;
}

bool ModuleUI::PreUpdate()
{
	bool ret = true;

	return ret;
}

bool ModuleUI::Update()
{
	bool ret = true;

	return ret;
}

bool ModuleUI::PostUpdate()
{
	bool ret = true;

	return ret;
}

bool ModuleUI::CleanUp()
{
	bool ret = true;

	DestroyAllUIElements();

	return ret;
}

UIElement * ModuleUI::CreateUIElement()
{
	UIElement* ret = new UIElement();

	elements.push_back(ret);

	return ret;
}

void ModuleUI::DestroyUIElement(UIElement * el)
{
	if (el != nullptr)
	{
		for (std::vector<UIElement*>::iterator it = elements.begin(); it != elements.end(); ++it)
		{
			if (el == (*it))
			{
				elements.erase(it);
				break;
			}
		}

		el->DestroyAllHandlers();
		RELEASE(el);
	}
}

std::vector<UIElement*> ModuleUI::GetAllElements() const
{
	return elements;
}

void ModuleUI::DestroyAllUIElements()
{
	for (std::vector<UIElement*>::iterator it = elements.begin(); it != elements.end(); ++it)
	{
		(*it)->DestroyAllHandlers();
		RELEASE(*it);
	}

	elements.clear();
}

UIHandler::UIHandler()
{
}

void UIHandler::SetTransfroms(const float2 & _pos, const float2 & _size, float _rotation_degrees)
{
	pos = _pos;
	size = _size;
	rotation_degrees = _rotation_degrees;

	float2 half_size = size * 0.5f;

	bbox.SetNegativeInfinity();
	bbox = AABB(float3(_pos.x - half_size.x, _pos.y - half_size.y, -1),
		float3(_pos.x + half_size.x, _pos.y + half_size.y, 1));

	bbox.SetNegativeInfinity();

	AABB abb = AABB(float3(_pos.x - half_size.x, _pos.y - half_size.y, -1),
		float3(_pos.x + half_size.x, _pos.y + half_size.y, 1));

	bbox.SetFrom(abb, Quat::FromEulerXYZ(0, 0, DEGTORAD * rotation_degrees));
}

void UIHandler::SetLayer(int set)
{
	layer = set;
}

bool UIHandler::CheckRay(const LineSegment & ray)
{
	return bbox.Intersects(ray);
}

float2 UIHandler::GetPos() const
{
	return pos;
}

float2 UIHandler::GetSize() const
{
	return size;
}

float UIHandler::GetRotationDegrees() const
{
	return rotation_degrees;
}

bool UIHandler::GetHovered() const
{
	return hovered;
}

bool UIHandler::GetPressed() const
{
	return pressed;
}

void UIHandler::SetActive(bool set)
{
	active = set;
}

bool UIHandler::GetActive() const
{
	return active;
}

UIElement::UIElement()
{
}

UIHandler * UIElement::AddHandler()
{
	UIHandler* ret = new UIHandler();
	ret->SetActive(enabled);

	handlers.push_back(ret);

	return ret;
}

UIHandler * UIElement::GetHandler(uint index) const
{
	UIHandler* ret = nullptr;

	if(handlers.size() - 1 <= index)
		ret = handlers[index];

	return ret;
}

std::vector<UIHandler*> UIElement::GetAllHandlers() const
{
	return handlers;
}

void UIElement::SetEnabled(bool set)
{
	enabled = set;

	for (std::vector<UIHandler*>::iterator it = handlers.begin(); it != handlers.end(); ++it)
	{
		(*it)->SetActive(enabled);
	}
}

void UIElement::DestroyAllHandlers()
{
	for (std::vector<UIHandler*>::iterator it = handlers.begin(); it != handlers.end(); ++it)
	{
		RELEASE(*it);
	}

	handlers.clear();
}
