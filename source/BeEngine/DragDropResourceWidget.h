#ifndef __DRAG_DROP_RESOURCE_WIDGET_H__
#define __DRAG_DROP_RESOURCE_WIDGET_H__

#include <string>
#include <vector>

#include "imgui.h"

class Resource;
enum ResourceType;

class DragDropResourceWidget
{
public:
	DragDropResourceWidget();

	bool DrawDragSource(const std::vector<Resource*>& dragging_res);
	bool DrawDragTarget(std::vector<Resource*>& dragging_res);

private:
	std::vector<Resource*> dragging;

	std::string payload_name = "Resource";

};

#endif // !__DRAG_DROP_RESOURCE_WIDGET_H__