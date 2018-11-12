#include "GameObjectComponent.h"

GameObjectComponent::GameObjectComponent(std::string _name, const ComponentType& _type, const ComponentGroup& _group, 
	bool _unique_per_game_object, bool _can_destroy)
{
	name = _name;
	type = _type;
	group = _group;
	unique_per_game_object = _unique_per_game_object;
	can_destroy = _can_destroy;
}

ComponentType GameObjectComponent::GetType() const
{
	return type;
}

std::string GameObjectComponent::GetName() const
{
	return name;
}
