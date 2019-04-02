#ifndef __DRAG_DROP_GAME_OBJECT_WIDGET_H__
#define __DRAG_DROP_GAME_OBJECT_WIDGET_H__

#include <string>
#include <vector>

#include "imgui.h"

class GameObject;
enum ResourceType;

class DragDropGameObjectWidget
{
public:
	DragDropGameObjectWidget();

	bool DrawDragSource(const std::vector<GameObject*>& dragging_gos);
	bool DrawDragTarget(std::vector<GameObject*>& returned_gos);

private:
	std::vector<GameObject*> dragging;

	std::string payload_name = "GameObjects";
	
};

#endif // !__DRAG_DROP_GAME_OBJECT_WIDGET_H__