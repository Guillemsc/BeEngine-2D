#ifndef __COMPONENT_CANVAS_H__
#define __COMPONENT_CANVAS_H__

#include "GameObjectComponent.h"

#include "GeometryMath.h"

class GameObject;
class Event;

class ComponentCanvas : public GameObjectComponent
{
	friend class GameObject;

private:
	void operator delete(void *) {}

public:
	ComponentCanvas();
	~ComponentCanvas();

	void EditorDraw();

	void Start();
	void Update();
	void CleanUp();

	void OnSaveAbstraction(DataAbstraction& abs);
	void OnLoadAbstraction(DataAbstraction& abs);

	void OnEvent(Event* ev);

	void OnChildAdded(GameObject* child);
	void OnChildRemoved(GameObject* child);
	void OnParentChanged(GameObject* new_parent);

	void RenderGuizmos(float relative_size);

	float2 GetCanvasCenter() const;
	float2 GetPositionFromAnchorPoint(const float2& anchor_point);
	float2 GetAnchorPointFromPosition(const float2& position);

private:
	void UpdateCanvasPosition();

	void UpdateCanvasLayout();

private:
	float2 reference_size = float2(1280, 720);
};

#endif // !__COMPONENT_CANVAS_H__