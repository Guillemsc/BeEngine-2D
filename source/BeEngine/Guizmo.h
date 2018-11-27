#ifndef __GUIZMO_H__
#define __GUIZMO_H__

#include "GeometryMath.h"
#include "Globals.h"

class GuizmoHandler;

class Guizmo
{
	friend class ModuleGuizmo;

public:
	Guizmo() {};
	~Guizmo() {};

	virtual void Start() {};
	virtual void Render(float relative_size) {};
	virtual bool UpdateTransform(float4x4& transform) { return false; };

	void CleanUp();

	uint GetHandlersCount() const;
	GuizmoHandler* GetHandler(uint index) const;

protected:
	void AddHandler();

	void SetMatPos(float4x4& transform, float2 pos, bool use_z_layer = false, float z_layer = 0);
	float2 GetMatPos(const float4x4& transform);
	float GetMatZLayer(const float4x4& transform);

	float2 GetInternalPos();
	float GetInternalZLayer();
	void SetInternalPos(const float2& pos);
	void AddInternalPos(const float2& to_add);

private:
	void DestroyAllHandlers();

protected:
	std::vector<GuizmoHandler*> handlers;

private:
	float4x4 internal_transform = float4x4::identity;
	bool	 visible = true;
	bool	 keep_size = true; 
};

class GuizmoHandler
{
	friend class ModuleGuizmo;

public:
	GuizmoHandler();
	~GuizmoHandler();

	void SetTransfroms(const float2& pos, const float2& size);
	bool CheckRay(const LineSegment& ray);

	float2 GetPos() const;
	float2 GetSize() const;

	bool GetHovered() const;
	bool GetPressed() const;

private:
	float2 pos = float2::zero;
	float2 size = float2::zero;

	bool hovered = false;
	bool pressed = false;

	AABB bbox;
};

#endif // !__GUIZMO_H__