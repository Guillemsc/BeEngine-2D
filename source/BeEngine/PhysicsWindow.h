#ifndef __PHYSICS_WINDOW_H__
#define __PHYSICS_WINDOW_H__

#include "ModuleEditor.h"
#include "imgui.h"

class PhysicsWindow : public EditorWindow
{
	friend class ModuleEditor;

private:
	void operator delete(void *) {}

public:
	PhysicsWindow();
	~PhysicsWindow();

	void Start();
	void CleanUp();
	void DrawEditor();
	ImGuiWindowFlags GetWindowFlags();

private:

};

#endif // !__PHYSICS_WINDOW_H__