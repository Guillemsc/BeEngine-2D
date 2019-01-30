#ifndef __GAME_OBJECT_ABSTRACTION_H__
#define __GAME_OBJECT_ABSTRACTION_H__

#include <vector>
#include <map>

#include "Globals.h"
#include "GeometryMath.h"
#include "ModuleJson.h"

class GameObject;
class JSON_Doc;
class DataAbstraction;
class GameObjectAbstraction;

class GameObjectAbstractionRelation
{
	friend GameObjectAbstraction;

private:
	void operator delete(void *) {}

private:
	uint id = 0;
	uint parent_id = 0;

	DataAbstraction go_abstraction;
	std::vector<DataAbstraction> components_abstraction;
};

class GameObjectAbstraction
{
	friend class GameObject;

private:
	void operator delete(void *) {}

public:
	GameObjectAbstraction();
	~GameObjectAbstraction();

	void Clear();

private:
	std::vector<GameObjectAbstractionRelation> relations;
};

#endif // !__GAME_OBJECT_COMPONENT_H__