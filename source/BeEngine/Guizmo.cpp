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

void Guizmo::SetMatPos(float4x4& transform, float2 new_pos, bool use_z_layer, float z_layer)
{
	float3 pos;
	Quat rot;
	float3 scal;

	transform.Decompose(pos, rot, scal);

	if (!use_z_layer)
		z_layer = pos.z;

	transform = float4x4::FromTRS(float3(new_pos.x, new_pos.y, z_layer), rot, scal);
}

float2 Guizmo::GetMatPos(const float4x4 & transform)
{
	float2 ret = float2::zero;

	Quat rot;
	float3 scale;
	float3 pos;
	transform.Decompose(pos, rot, scale);

	ret = float2(pos.x, pos.y);

	return ret;
}

float Guizmo::GetMatZLayer(const float4x4 & transform)
{
	float ret = 0;

	Quat rot;
	float3 scale;
	float3 pos;
	transform.Decompose(pos, rot, scale);

	ret = pos.z;

	return ret;
}

float2 Guizmo::GetInternalPos()
{
	return GetMatPos(internal_transform);
}

float Guizmo::GetInternalZLayer()
{
	return GetMatZLayer(internal_transform);
}

void Guizmo::SetInternalPos(const float2 & pos)
{
	SetMatPos(internal_transform, pos, true, 100);
}

void Guizmo::AddInternalPos(const float2& to_add)
{
	Quat rot = Quat::identity;
	float3 scale = float3::one;
	float3 new_to_add = float3(to_add.x, to_add.y, 0);
	float4x4 to_add_mat = float4x4::FromTRS(new_to_add, rot, scale);

	internal_transform = internal_transform * to_add_mat;
}
