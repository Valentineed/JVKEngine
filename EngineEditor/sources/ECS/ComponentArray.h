#pragma once
#include <cassert>
#include <unordered_map>

#include "CommonTypes.h"
class IComponentArray
{
public:
	virtual ~IComponentArray() = default;
	virtual void EntityDestroyed(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray
{
public:
	void InsertData(Entity entity, T&& component);
	void InsertDataID(Entity entity, size_t index);

	//Todo add remove Component?
	void RemoveData(Entity entity);

	void RemoveDataID(Entity entity);

	void ClearDataID(Entity entity);

	T* GetData(Entity entity);
	size_t GetDataID(Entity entity);

	void EntityDestroyed(Entity entity) override;

	template<typename ...Args>
	void EmplaceData(Entity entity, Args&&... args);

private:
	std::array<T, MAX_ENTITIES> m_componentArray;
	
	
	std::array<size_t, MAX_ENTITIES> m_componentIDArray;

	//Map from an entity ID to an array index
	std::unordered_map<Entity, size_t> m_entityToIndexMap;

	//Map from an array index to an entity ID 
	std::unordered_map<size_t, Entity> m_indexToEntityMap;

	// Total size of valid entries in the array.
	size_t m_size{};
};

//TODO deplace in .inl ?
template<typename T>
void ComponentArray<T>::InsertData(Entity entity, T&& component)
{
	assert(m_entityToIndexMap.find(entity) == m_entityToIndexMap.end() && "Component added to same entity more than once.");

	// Put new entry at end and update the maps
	size_t newIndex = m_size;
	m_entityToIndexMap[entity] = newIndex;
	m_indexToEntityMap[newIndex] = entity;
	m_componentArray[newIndex] = std::move(component);
	++m_size;
}

template<typename T>
inline void ComponentArray<T>::InsertDataID(Entity entity, size_t index)
{
	// Put new entry at end and update the maps
	size_t newIndex = m_size;
	m_entityToIndexMap[entity] = newIndex;
	m_indexToEntityMap[newIndex] = entity;
	m_componentIDArray[newIndex] = index;
	++m_size;
}

template<typename T>
void ComponentArray<T>::RemoveData(Entity entity)
{
	assert(m_entityToIndexMap.find(entity) != m_entityToIndexMap.end() && "Removing non-existent component.");

	// Copy element at end into deleted element's place to maintain density
	size_t indexOfRemovedEntity = m_entityToIndexMap[entity];
	size_t indexOfLastElement = m_size - 1;
	m_componentArray[indexOfRemovedEntity] = std::move(m_componentArray[indexOfLastElement]);

	// Update map to point to moved spot
	Entity entityOfLastElement = m_indexToEntityMap[indexOfLastElement];
	m_entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
	m_indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

	m_entityToIndexMap.erase(entity);
	m_indexToEntityMap.erase(indexOfLastElement);

	--m_size;
}

template<typename T>
void ComponentArray<T>::RemoveDataID(Entity entity)
{
	assert(m_entityToIndexMap.find(entity) != m_entityToIndexMap.end() && "Removing non-existent component.");

	// Copy element at end into deleted element's place to maintain density
	size_t indexOfRemovedEntity = m_entityToIndexMap[entity];
	size_t indexOfLastElement = m_size - 1;
	//m_componentIDArray[indexOfRemovedEntity] = m_componentIDArray[indexOfLastElement];

	// Update map to point to moved spot
	Entity entityOfLastElement = m_indexToEntityMap[indexOfLastElement];
	m_entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
	m_indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

	m_entityToIndexMap.erase(entity);
	m_indexToEntityMap.erase(indexOfLastElement);

	--m_size;
}

template <typename T>
void ComponentArray<T>::ClearDataID(Entity entity)
{
	assert(m_entityToIndexMap.find(entity) != m_entityToIndexMap.end() && "Removing non-existent component.");
	
	m_entityToIndexMap.erase(entity);
	m_indexToEntityMap.erase(entity);
	
	--m_size;
}

template<typename T>
T* ComponentArray<T>::GetData(Entity entity)
{	
	if (m_entityToIndexMap.find(entity) == m_entityToIndexMap.end())
	{
		return nullptr;
	}

	return &m_componentArray[m_entityToIndexMap[entity]];
}

template<typename T>
inline size_t ComponentArray<T>::GetDataID(Entity entity)
{
	if (m_entityToIndexMap.find(entity) == m_entityToIndexMap.end())
	{
		return -1;
	}

	return m_componentIDArray[m_entityToIndexMap[entity]];
}

template<typename T>
void ComponentArray<T>::EntityDestroyed(Entity entity)
{
	if (m_entityToIndexMap.find(entity) != m_entityToIndexMap.end())
	{
		RemoveData(entity);
	}
}

template<typename T>
template<typename ...Args>
inline void ComponentArray<T>::EmplaceData(Entity entity, Args&& ...args)
{
	assert(m_entityToIndexMap.find(entity) == m_entityToIndexMap.end() && "Component added to same entity more than once.");

	// Put new entry at end and update the maps
	size_t newIndex = m_size;
	m_entityToIndexMap[entity] = newIndex;
	m_indexToEntityMap[newIndex] = entity;
	m_componentArray[newIndex] = T(std::forward<Args>(args)...);
	++m_size;
}
