#ifndef __GAME_OBJECT_COMPONENT_H__
#define __GAME_OBJECT_COMPONENT_H__

#include <vector>

#include "Globals.h"
#include "GameObject.h"

class GameObjectComponent
{
public:
	GameObjectComponent() {};
	~GameObjectComponent() {};

	virtual void Start() {};
	virtual void CleanUp() {};

	virtual void OnChildAdded(GameObject* child) {};
	virtual void OnChildRemoved(GameObject* child) {};
	virtual void OnParentChanged(GameObject* new_parent) {};
};

#endif // !__GAME_OBJECT_COMPONENT_H__