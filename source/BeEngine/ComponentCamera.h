#ifndef __COMPONENT_CAMERA_H__
#define __COMPONENT_CAMERA_H__

#include "GameObjectComponent.h"
#include "GeometryMath.h"

class GameObject;
class Event;
class Camera2D;

class ComponentCamera : public GameObjectComponent
{
	friend class GameObject;

private:
	void operator delete(void *) {}

public:
	ComponentCamera();
	~ComponentCamera();

	void EditorDraw();

	void Start();
	void Update();
	void CleanUp();

	void OnSaveAbstraction(DataAbstraction& abs);
	void OnLoadAbstraction(DataAbstraction& abs);

	void OnEvent(Event* ev);

	void OnDestroy();
	void OnChildAdded(GameObject* child);
	void OnChildRemoved(GameObject* child);
	void OnParentChanged(GameObject* new_parent);

	void RenderGuizmos(float relative_size);

	Camera2D* GetCamera();

private:
	void UpdateCameraPos();

private:
	Camera2D* camera = nullptr;
};

#endif // !__COMPONENT_CAMERA_H__