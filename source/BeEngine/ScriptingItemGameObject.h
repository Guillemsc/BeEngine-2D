#ifndef __SCRIPTING_ITEM_GAME_OBJECT_H__
#define __SCRIPTING_ITEM_GAME_OBJECT_H__

#include "ScriptingItem.h"

class GameObject;
class Event;

class ScriptingItemGameObject : public ScriptingItem
{
	friend class ModuleScripting;

private:
	void operator delete(void *) {}

public:
	ScriptingItemGameObject();
	~ScriptingItemGameObject();

	void Start();
	void RegisterInternalCalls();
	void CleanUp();

	void OnEvent(Event* ev);

	void RebuildInstances();
	void DestroyInstances();

private:
	void AddScriptingInstance(GameObject* go);
	void RemoveScriptingInstance(GameObject* go);
};

#endif // !__SCRIPTING_ITEM_GAME_OBJECT_H__