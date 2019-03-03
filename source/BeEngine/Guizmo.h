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

	void CleanUp();

	uint GetHandlersCount() const;
	GuizmoHandler* GetHandler(uint index) const;

protected:
	void AddHandler();

	void DestroyAllHandlers();

protected:
	std::vector<GuizmoHandler*> handlers;

private:
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