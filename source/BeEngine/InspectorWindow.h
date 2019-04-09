#ifndef __INSPECTOR_WINDOW_H__
#define __INSPECTOR_WINDOW_H__

#include "ModuleEditor.h"
#include "imgui.h"

class GameObject;
class Resource;
class Event;

enum InspectorWindowShowing
{
	SHOWING_GAMEOBJECTS,
	SHOWING_RESOURCES,
};

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

	void SetShowingResources(const std::vector<Resource*>& res);
	void SetShowingResource(Resource* res);
	void RemoveFromShowingResources(Resource * res);

private:
	void OnEvent(Event* ev);

	void DrawGameObjects();
	void DrawResources();

	void DrawComponentsPopup(const std::vector<GameObject*>& selected_gos);

private:
	ImFont* font = nullptr;

	InspectorWindowShowing showing = InspectorWindowShowing::SHOWING_GAMEOBJECTS;

	std::vector<GameObject*> showing_gos;
	std::vector<Resource*> showing_resources;
};

#endif // !__INSPECTOR_WINDOW_H__