#include "SystemManager.h"

void SystemManager::EntityDestroyed(Entity entity)
{
	for (auto const& pair : m_systems)
	{
		auto const& system = pair.second;

		system->m_entities.erase(entity);
	}
}

void SystemManager::EntitySignatureChanged(Entity entity, Signature entitySignature)
{
	for (auto const& pair : m_systems)
	{
		auto const& type = pair.first;
		auto const& system = pair.second;
		auto const& systemSignature = m_signatures[type];

		if ((entitySignature & systemSignature) == systemSignature)
		{
			system->m_entities.insert(entity);
		}
		else
		{
			system->m_entities.erase(entity);
		}
	}
}
