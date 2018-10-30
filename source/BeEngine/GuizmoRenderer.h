#ifndef __GUIZMO_RENDERER_H__
#define __GUIZMO_RENDERER_H__

#include "GeometryMath.h"

class GuizmoRenderer
{
public:
	GuizmoRenderer() {};
	~GuizmoRenderer() {};

	virtual void Start() {};
	virtual void CleanUp() {};
	virtual void Render(const float4x4& view, const float4x4& projection) {};
};

#endif // !__GUIZMO_RENDERER_H__