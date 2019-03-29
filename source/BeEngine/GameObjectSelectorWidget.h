#ifndef __GAME_OBJECT_SELECTOR_WIDGET_H__
#define __GAME_OBJECT_SELECTOR_WIDGET_H__

#include <string>

#include "imgui.h"

class GameObject;
enum ResourceType;

class GameObjectSelectorWidget
{

public:
	GameObjectSelectorWidget();

	bool Draw(const std::string& selector_name, GameObject *& go);

private:
	std::string filter_text;
};

#endif // !__GAME_OBJECT_SELECTOR_WIDGET_H__