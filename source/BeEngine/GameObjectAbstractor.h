#ifndef __GAME_OBJECT_ABSTRACTION_H__
#define __GAME_OBJECT_ABSTRACTION_H__

#include <vector>
#include <map>

#include "Globals.h"
#include "GeometryMath.h"

class GameObject;
class JSON_Doc;
class DataAbstraction;

class GoToIdRelation
{
	friend GameObjectAbstraction;

public:
	GoToIdRelation() {}
	GoToIdRelation(uint _id, GameObject* _go, const DataAbstraction& abs, const std::vector<DataAbstraction>& components_abs);
	GoToIdRelation(uint _id, const DataAbstraction& abs, const std::vector<DataAbstraction>& components_abs);

	void SetGo(GameObject* go);

	uint GetId() const;
	DataAbstraction GetDataAbstraction() const;
	std::vector<DataAbstraction> GetComponentsDataAbstraction() const;
	GameObject* GetGo() const;

private:
	uint id = 0;
	DataAbstraction data_game_object;
	std::vector<DataAbstraction> data_components;

	GameObject* go = nullptr;
};

class ChildToParentRelation
{
	friend GameObjectAbstraction;

public:
	ChildToParentRelation(uint _id, uint _id_parent);

	uint GetId() const;
	uint GetParentId() const;

private:
	uint id = 0;
	uint id_parent = 0;
};

class GameObjectAbstraction
{
	friend class GameObject;
	friend class GORelation;

private:
	void operator delete(void *) {}

public:
	GameObjectAbstraction();
	~GameObjectAbstraction();

	void Clear();

	void Abstract(GameObject* go);
	GameObject* DeAbstract();

	void Serialize(const std::string& path, const std::string& name, const std::string& extension);
	void DeSerialize(const std::string& filepath);

private:
	// Abstraction
	void AddGoToIdRelation(GameObject* go, uint& curr_id);
	void AddIdToAbstractionRelation(uint id, DataAbstraction go_abs, std::vector<DataAbstraction> components_abs);
	void AddChildToParentRelation(uint id, uint parent_id);

	// DeAbstraction
	int GetParentFromChild(int child_id);
	GameObject* GetGoToIdRelation(int id);
	int GetIdToGoRelation(GameObject* go);

private:
	std::vector<GoToIdRelation> go_to_ids_relations;
	std::vector<ChildToParentRelation> child_to_parent_relations;

	bool loaded = false;
};

#endif // !__GAME_OBJECT_COMPONENT_H__