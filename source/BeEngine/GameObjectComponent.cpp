#include "GameObjectComponent.h"

#include "mmgr\nommgr.h"
#include "mmgr\mmgr.h"

GameObjectComponent::GameObjectComponent(ScriptingBridgeObject* scripting_bridge, 
	std::string _name, const ComponentType& _type, const ComponentGroup& _group, bool _unique_per_game_object, bool _can_destroy) : 
	BeObject(scripting_bridge)
{
	name = _name;
	type = _type;
	group = _group;
	unique_per_game_object = _unique_per_game_object;
	can_destroy = _can_destroy;
}

GameObject * GameObjectComponent::GetOwner() const
{
	return owner;
}

ComponentType GameObjectComponent::GetType() const
{
	return type;
}

ComponentGroup GameObjectComponent::GetGroup() const
{
	return group;
}

std::string GameObjectComponent::GetName() const
{
	return name;
}

std::string GameObjectComponent::GetUID() const
{
	return uid;
}

bool GameObjectComponent::GetCanDestroy() const
{
	return can_destroy;
}
