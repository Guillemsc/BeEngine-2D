#ifndef __COMPONENT_TRANSFORM_H__
#define __COMPONENT_TRANSFORM_H__

#include "GameObjectComponent.h"

#include "GeometryMath.h"

class GameObject;
class ScriptingBridgeComponentTransform;
class PhysicsBody;
class ComponenPhysicsBody;
class ComponentCanvas;

class ComponentTransform : public GameObjectComponent
{
	friend class GameObject;
	friend class ComponentPhysicsBody;
	friend class ComponentPolygonCollider;
	friend class ComponentCanvas;

private:
	void operator delete(void *) {}

public:
	ComponentTransform();
	~ComponentTransform();

	void EditorDraw();

	void Start();
	void Update();
	void CleanUp();

	void RenderGuizmos(float relative_size);

	void OnSaveAbstraction(DataAbstraction& abs);
	void OnLoadAbstraction(DataAbstraction& abs);

	void OnOwnerSelected();
	void OnOwnerDeSelected();

	void OnChildAdded(GameObject* child);
	void OnChildRemoved(GameObject* child);
	void OnParentChanged(GameObject* new_parent);

	void OnAddComponent(GameObjectComponent* new_component);
	void OnRemoveComponent(GameObjectComponent* new_component);

	void SetLocalTransform(const float4x4& local);
	void SetWorldTransform(const float4x4& world);

	void SetLocalPosition(const float2& pos);
	void SetLocalRotationDegrees(float rotation);
	void SetLocalScale(const float2& scale);

	void SetPosition(const float2& pos);
	void SetRotationDegrees(float rotation);
	void SetScale(const float2& scale);

	float2 GetLocalPosition() const;
	float GetLocalRotationDegrees() const;
	float2 GetLocalScale() const;

	float2 GetPosition() const;
	float GetRotationDegrees() const;
	float2 GetScale() const;

	float4x4 GetLocalTransform() const;
	float4x4 GetWorldTransform();

	void SetAnchorPos(const float2& anchor);
	void SetAnchorOffsetPos(const float2& anchor_offset);

	float2 GetAnchorPos() const;

	ComponentCanvas* GetUsedCanvas() const;

	ScriptingBridgeComponentTransform* GetScriptingBridge() const;

private:
	void UpdateLocalFromWorldTransform();
	void UpdateWorldFromLocalTransform();
	void UpdateWorldAndLocalValues();

	void UpdateLocalTransformFromValues();
	void UpdateWorldTransformFromValues();

	void UpdatePhysicsMovement();

	void FindParentUsedCanvas();
	void RecalculateCanvasLayout();

private:
	float4x4 local_transform;
	float4x4 world_transform;

	float2 local_pos = float2::zero;
	float  local_rotation = 0;
	float2 local_scale = float2::one;

	float2 world_scale = float2::zero;

	bool keep_scale_ratio = false;

	float2 anchor_pos = float2::zero;
	float2 anchor_offset_pos = float2::zero;

	PhysicsBody* base_physics_body = nullptr;
	ComponentPhysicsBody* used_physics_body_comp = nullptr;
	ComponentCanvas* used_canvas_comp = nullptr;

	ComponentCanvas* parent_used_canvas = nullptr;

private:
	ScriptingBridgeComponentTransform* scripting_bridge = nullptr;

	float2 last_pos = float2::zero;
	float last_rotation = 0;
};

#endif // !__COMPONENT_TRANSFORM_H__