#include "ModuleUI.h"
#include "App.h"
#include "ModuleEditor.h"
#include "GameWindow.h"
#include "ModuleCamera.h"
#include "ModuleInput.h"

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

	UpdateUIElements();

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

void ModuleUI::UpdateUIElements()
{
	Camera2D* game_camera = App->camera->GetGameCamera();

	if (game_camera != nullptr)
	{
		LineSegment ls = game_camera->ShootRay(App->editor->game_window->GetGameRect(), App->input->GetMouse());

		bool inside_window = App->editor->game_window->GetMouseInsideWindow();

		UIHandler* handler_on_top = nullptr;
		int handler_on_top_layer = -1;
		bool using_pressed = false;

		for (std::vector<UIElement*>::iterator it = elements.begin(); it != elements.end(); ++it)
		{
			std::vector<UIHandler*> handlers = (*it)->GetAllHandlers();

			for (std::vector<UIHandler*>::iterator ha = handlers.begin(); ha != handlers.end(); ++ha)
			{
				UIHandler* curr_handler = (*ha);

				if (curr_handler->GetActive() && inside_window)
				{
					if (curr_handler->CheckRay(ls) && curr_handler->GetLayer() > handler_on_top_layer)
					{						
						if (handler_on_top != nullptr && !handler_on_top->pressed)
						{
							handler_on_top->pressed = false;
							handler_on_top->hovered = false;
						}

						handler_on_top = curr_handler;
						handler_on_top_layer = curr_handler->GetLayer();
					}
					else if(curr_handler->pressed && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
					{
						curr_handler->pressed = true;
						curr_handler->hovered = true;

						using_pressed = true;
					}
					else
					{
						curr_handler->pressed = false;
						curr_handler->hovered = false;
					}
				}
				else
				{
					curr_handler->pressed = false;
					curr_handler->hovered = false;
				}
			}
		}

		if (handler_on_top != nullptr && !using_pressed)
		{						
			handler_on_top->hovered = true;

			if (inside_window)
			{
				if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
					handler_on_top->pressed = true;

				else if (App->input->GetMouseButton(SDL_BUTTON_LEFT) != KEY_REPEAT)
					handler_on_top->pressed = false;
			}
			else
			{
				handler_on_top->pressed = false;
				handler_on_top->hovered = false;
			}
		}
	}
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

	AABB abb = AABB(float3(_pos.x - half_size.x, _pos.y - half_size.y, -1),
		float3(_pos.x + half_size.x, _pos.y + half_size.y, 1));

	bbox.SetNegativeInfinity();

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

int UIHandler::GetLayer() const
{
	return layer;
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

	ret->SetTransfroms(float2(0, 0), float2(1, 1), 0);

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
