#ifndef __POSITION_GUIZMO_H__
#define __POSITION_GUIZMO_H__

#include "GeometryMath.h"
#include "Guizmo.h"

enum PositionGuizmoHandlers
{
	UP_ARROW,
	RIGHT_ARROW,
	CENTER,
};

class PositionGuizmo : public Guizmo
{
public:
	PositionGuizmo() {};
	~PositionGuizmo() {};

	void Start();
	void Render(float relative_size, const float2& mouse_pos);
	bool UpdateTransform(float4x4& transform);

	void SetMatPos(float4x4& transform, float2 pos);
	float2 GetMatPos(const float4x4& transform);

	float2 GetInternalPos();
	void SetInternalPos(const float2& pos);
	void AddInternalPos(const float2& to_add);

private:
	float4x4 internal_transform = float4x4::identity;

	bool edited = false;
};

#endif // !__POSITION_GUIZMO_H__