#include "Guizmo.h"

GuizmoHandler::GuizmoHandler()
{
}

GuizmoHandler::~GuizmoHandler()
{
}

void GuizmoHandler::SetTransfroms(const float2 & _pos, const float2 & _size)
{
	pos = _pos;
	size = _size;

	float2 half_size = size * 0.5f;

	bbox.SetNegativeInfinity();
	bbox = AABB(float3(_pos.x - half_size.x, _pos.y - half_size.y, -1), float3(_pos.x + half_size.x, _pos.y + half_size.y, 1));
}

bool GuizmoHandler::CheckRay(const LineSegment & ray)
{
	return bbox.Intersects(ray);
}

float2 GuizmoHandler::GetPos() const
{
	return pos;
}

float2 GuizmoHandler::GetSize() const
{
	return size;
}

bool GuizmoHandler::GetHovered() const
{
	return hovered;
}

bool GuizmoHandler::GetPressed() const
{
	return pressed;
}

void GuizmoHandler::SetActive(bool set)
{
	active = set;
}

void Guizmo::CleanUp()
{
	DestroyAllHandlers();
}

uint Guizmo::GetHandlersCount() const
{
	return handlers.size();
}

GuizmoHandler* Guizmo::GetHandler(uint index) const
{
	return handlers[index];
}

void Guizmo::SetEnabled(bool set)
{
	enabled = set;
}

GuizmoHandler* Guizmo::AddHandler()
{
	GuizmoHandler* ret = new GuizmoHandler();

	handlers.push_back(ret);

	return ret;
}

void Guizmo::DestroyAllHandlers()
{
	for (std::vector<GuizmoHandler*>::iterator it = handlers.begin(); it != handlers.end(); ++it)
	{
		RELEASE(*it);
	}

	handlers.clear();
}