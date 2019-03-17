#ifndef __CANVAS_ITEM_GUIZMO_H__
#define __CANVAS_ITEM_GUIZMO_H__

#include "GeometryMath.h"
#include "Guizmo.h"

class ComponentTransform;
class GameObject;

enum CanvasItemGuizmoHandlers
{
	ANCHOR,
};

class CanvasItemGuizmo : public Guizmo
{
public:
	CanvasItemGuizmo() {};
	~CanvasItemGuizmo() {};

	void Start();
	void Render(float relative_size, const float2& mouse_pos);

	void StartEditing(ComponentTransform* canvas_item_editing);
	void StopEditing(ComponentTransform* canvas_item_editing);
	ComponentTransform* GetEditingComponent() const;

private:
	void CreateHandlers();
	void DestroyHandlers();

private:
	ComponentTransform* canvas_item_editing = nullptr;
};

#endif // !__CANVAS_ITEM_GUIZMO_H__