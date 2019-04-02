#ifndef __INSPECTOR_WINDOW_H__
#define __INSPECTOR_WINDOW_H__

#include "ModuleEditor.h"
#include "imgui.h"

class GameObject;
class Event;

class InspectorWindow : public EditorWindow
{
	friend class ModuleEditor;

private:
	void operator delete(void *) {}

public:
	InspectorWindow();
	~InspectorWindow();

	void Start();
	void CleanUp();
	void DrawEditor();
	ImGuiWindowFlags GetWindowFlags();

	void SetShowingGos(const std::vector<GameObject*>& gos);
	void SetShowingGos(GameObject* go);
	void RemoveFromShowingGos(GameObject* go);

private:
	void OnEvent(Event* ev);

	void DrawComponentsPopup(const std::vector<GameObject*>& selected_gos);

private:
	ImFont* font = nullptr;

	std::vector<GameObject*> showing_gos;
};

#endif // !__INSPECTOR_WINDOW_H__