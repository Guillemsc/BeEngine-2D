#ifndef __RESOURCE_SELECTOR_WIDGET_H__
#define __RESOURCE_SELECTOR_WIDGET_H__

#include <string>

#include "imgui.h"

class Resource;
enum ResourceType;

class ResourceSelectorWidget 
{

public:
	ResourceSelectorWidget();

	bool Draw(const std::string& selector_name, const ResourceType type, Resource *& res);

private:
	std::string filter_text;
};

#endif // !__RESOURCE_SELECTOR_WIDGET_H__