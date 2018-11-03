#ifndef __MODULE_GUIZMO_H__
#define __MODULE_GUIZMO_H__

#include "Module.h"
#include "Guizmo.h"

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
	void AddGuizmo(Guizmo* add);
	void DestroyAllGuizmos();

private:
	std::vector<Guizmo*> guizmos;

	bool render_handlers = true;
};

#endif // !__MODULE_GUIZMO_H__