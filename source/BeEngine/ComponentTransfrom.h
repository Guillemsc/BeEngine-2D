#ifndef __COMPONENT_TRANSFORM_H__
#define __COMPONENT_TRANSFORM_H__

#include "GameObjectComponent.h"

#include "GeometryMath.h"

class GameObject;

class ComponentTransform : public GameObjectComponent
{
	friend class GameObject;

private:
	void operator delete(void *) {}

public:
	ComponentTransform();
	~ComponentTransform();

	void OnEditorDraw();

	void Start();
	void CleanUp();

	void OnChildAdded(GameObject* child);
	void OnChildRemoved(GameObject* child);
	void OnParentChanged(GameObject* new_parent);

	void SetPosition(const float2& pos);
	void SetRotationAngles(float rotation);
	void SetScale(const float2& scale);

	float2 GetPosition() const;
	float GetRotation() const;
	float2 GetScale() const;

	float4x4 GetLocalTransform() const;
	float4x4 GetGlobalTransform() const;

private:
	void UpdateLocalTransform();

	void UpdateChildsTreeGlobalTransform();

private:
	float4x4 local_transform;

	float2 local_pos = float2::zero;
	float local_rotation = 0;
	float2 local_scale = float2::zero;

	float4x4 global_transform;
};

#endif // !__COMPONENT_TRANSFORM_H__