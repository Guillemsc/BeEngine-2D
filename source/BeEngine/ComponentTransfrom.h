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

	void EditorDraw();

	void Start();
	void CleanUp();

	void OnSaveAbstraction(DataAbstraction& abs);
	void OnLoadAbstraction(DataAbstraction& abs);

	void OnChildAdded(GameObject* child);
	void OnChildRemoved(GameObject* child);
	void OnParentChanged(GameObject* new_parent);

	void SetLocalTransform(const float4x4& local);
	void SetWorldTransform(const float4x4& world);

	void SetLocalPosition(const float2& pos);
	void SetLocalRotationDegrees(float rotation);
	void SetLocalScale(const float2& scale);

	void SetPosition(const float2& pos);
	void SetRotationDegrees(float rotation);
	void SetScale(const float2& scale);

	float2 GetLocalPosition();
	float GetLocalRotationDegrees();
	float2 GetLocalScale();

	float2 GetPosition();
	float GetRotationDegrees();
	float2 GetScale();

	float4x4 GetLocalTransform();
	float4x4 GetWorldTransform();

private:
	void UpdateLocalFromWorldTransform();
	void UpdateWorldFromLocalTransform();
	void UpdateWorldAndLocalValues();

	void UpdateLocalTransformFromValues();
	void UpdateWorldTransformFromValues();

private:
	float4x4 local_transform;
	float4x4 world_transform;

	float2 local_pos = float2::zero;
	float  local_rotation = 0;
	float2 local_scale = float2::one;

	float2 world_pos = float2::zero;
	float  world_rotation = 0;
	float2 world_scale = float2::zero;

	bool keep_scale_ratio = false;
};

#endif // !__COMPONENT_TRANSFORM_H__