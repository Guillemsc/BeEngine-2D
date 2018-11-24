#ifndef __MODULE_GUIZMO_H__
#define __MODULE_GUIZMO_H__

#include "Module.h"
#include "Guizmo.h"

class PositionGuizmo;

class ModuleGuizmo : public Module
{
public:
	ModuleGuizmo();
	~ModuleGuizmo();

	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update();
	bool PostUpdate();
	bool CleanUp();

	void RenderGuizmos();

	void SetRenderHandlers(bool set);
	bool GetRenderHandlers() const;

private:
	Guizmo* AddGuizmo(Guizmo* add);
	void DestroyAllGuizmos();

private:
	std::vector<Guizmo*> guizmos;

	PositionGuizmo* position_guizmo = nullptr;

	bool render_handlers = false;
};

#endif // !__MODULE_GUIZMO_H__