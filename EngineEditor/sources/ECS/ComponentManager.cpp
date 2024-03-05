#include "ComponentManager.h"

void ComponentManager::EntityDestroyed(Entity entity)
{
	for (auto const& pair : m_componentArrays)
	{
		auto const& component = pair.second;

		component->EntityDestroyed(entity);
	}
}
