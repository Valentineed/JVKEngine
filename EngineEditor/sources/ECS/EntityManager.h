#pragma once
#include <array>
#include <queue>

#include "CommonTypes.h"
/*The Entity Manager is in charge of distributing
 *entity IDs and keeping record of which IDs are in
 *use and which are not.*/
class EntityManager
{
public:
	EntityManager();

	Entity CreateEntity();

	void DestroyEntity(Entity entity);

	void SetSignature(Entity entity, Signature signature);

	Signature GetSignature(Entity entity);

private:
	// Queue of unused entity IDs
	std::queue<Entity> m_availableEntities{};
	
	// Array of signatures where the index corresponds to the entity ID
	std::array<Signature, MAX_ENTITIES> m_signatures{};

	// Total living entities - used to keep limits on how many exist
	uint32_t m_livingEntityCount{};
};
