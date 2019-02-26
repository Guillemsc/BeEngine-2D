#ifndef __GRID_GUIZMO_H__
#define __GRID_GUIZMO_H__

#include "GeometryMath.h"
#include "Guizmo.h"

class GridGuizmo : public Guizmo
{
public:
	GridGuizmo() {};
	~GridGuizmo() {};

	void Start();
	void Render(float relative_size);
	bool UpdateTransform(float4x4& transform);

private:

};

#endif // !__POSITION_GUIZMO_H__