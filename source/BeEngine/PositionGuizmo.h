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
	void Render(float relative_size);
	void UpdateTransform(float4x4& transform);

private:
	float2 GetPosition();
	void AddPosition(const float2& add);

private:
	float4x4 internal_transform = float4x4::identity;
};

#endif // !__POSITION_GUIZMO_H__