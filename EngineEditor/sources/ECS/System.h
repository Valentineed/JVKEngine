#pragma once
#include "CommonTypes.h"
#include <set>

//https://austinmorlan.com/posts/entity_component_system/
/*chose a std::set rather than a std::list for a few reasons.

First, each entity is unique, and a set is defined as having
every element be unique, so it maps well logically.

Second, each entity is an integer which makes for easy
compares when inserting/removing from the set. Removing
a specific entity from a list is O(n) because you have to start
at the beginning and possibly go to the end, while removing from
a set is O(log n) because it’s a binary tree. However,
inserting into a list is only O(1) while inserting
into a set is O(log n) as well.*/
class System
{
public:
	std::set<Entity> m_entities;
};
