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

	bbox.SetNegativeInfinity();
	bbox = AABB(float3(_pos.x - size.x, _pos.y - size.y, -1), float3(_pos.x + size.x, _pos.y + size.y, 1));
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

void Guizmo::AddHandler()
{
	handlers.push_back(new GuizmoHandler());
}

void Guizmo::DestroyAllHandlers()
{
	for (std::vector<GuizmoHandler*>::iterator it = handlers.begin(); it != handlers.end(); ++it)
	{
		RELEASE(*it);
	}

	handlers.clear();
}
