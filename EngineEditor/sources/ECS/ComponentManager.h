#pragma once
#include <cassert>
#include <memory>

#include "CommonTypes.h"
#include "ComponentArray.h"
/*talk to all of the different ComponentArrays when a component needs to be added or removed*/
class ComponentManager
{
public:
	template<typename T>
	void RegisterComponent();

	template<typename T>
	ComponentType GetComponentType();

	template<typename T>
	void AddComponent(Entity entity, T&& component);

	template<typename T>
	void AddComponentID(Entity entity, size_t index);

	template<typename T>
	void RemoveComponent(Entity entity);

	template<typename T>
	void RemoveComponentID(Entity entity);
	
	template<typename T>
	void ClearComponentID(Entity entity);	
	
	template<typename T>
	T* GetComponent(Entity entity);

	template<typename T>
	size_t GetComponentID(Entity entity);
	
	void EntityDestroyed(Entity entity);

	template<typename T, typename...Args>
	void EmplaceComponent(Entity entity, Args&&... args);

	


	

private:/*
	std::unordered_map<size_t, ComponentType> m_componentTypes{};
	std::unordered_map<size_t, std::shared_ptr<IComponentArray>> m_componentArrays{};*/
	std::unordered_map<const char*, ComponentType> m_componentTypes{};
	std::unordered_map<const char*, std::shared_ptr<IComponentArray>> m_componentArrays{};
	ComponentType m_nextComponentType{};


	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray();
 
};

//TODO deplace in .inl ?
template <typename T>
void ComponentManager::RegisterComponent()
{
	//size_t typeName = HashString::Hash(typeid(T).name());
	const char* typeName = typeid(T).name();

	assert(m_componentTypes.find(typeName) == m_componentTypes.end() && "Registering component type more than once.");

	m_componentTypes.insert({ typeName, m_nextComponentType });
	m_componentArrays.insert({ typeName, std::make_shared<ComponentArray<T>>() });

	++m_nextComponentType;
}

template <typename T>
ComponentType ComponentManager::GetComponentType()
{
	//size_t typeName = HashString::Hash(typeid(T).name());
	const char* typeName = typeid(T).name();

	assert(m_componentTypes.find(typeName) != m_componentTypes.end() && "Component not registered before use.");

	return m_componentTypes[typeName];
}

template <typename T>
void ComponentManager::AddComponent(Entity entity, T&& component)
{
	GetComponentArray<T>()->InsertData(entity, std::move(component));
}

template<typename T>
inline void ComponentManager::AddComponentID(Entity entity, size_t index)
{
	GetComponentArray<T>()->InsertDataID(entity, index);
}

template <typename T>
void ComponentManager::RemoveComponent(Entity entity)
{
	GetComponentArray<T>()->RemoveData(entity);
}

template <typename T>
void ComponentManager::RemoveComponentID(Entity entity)
{
	GetComponentArray<T>()->RemoveDataID(entity);
}

template <typename T>
void ComponentManager::ClearComponentID(Entity entity)
{
	GetComponentArray<T>()->ClearDataID(entity);
}

template <typename T>
T* ComponentManager::GetComponent(Entity entity)
{
	return GetComponentArray<T>()->GetData(entity);
}

template<typename T>
inline size_t ComponentManager::GetComponentID(Entity entity)
{
	return GetComponentArray<T>()->GetDataID(entity);
}

template<typename T, typename ...Args>
inline void ComponentManager::EmplaceComponent(Entity entity, Args&& ...args)
{
	GetComponentArray<T>()->EmplaceData(entity, std::forward<Args>(args)...);
}

template <typename T>
std::shared_ptr<ComponentArray<T>> ComponentManager::GetComponentArray()
{
	//size_t typeName = HashString::Hash(typeid(T).name());
	const char* typeName = typeid(T).name();

	assert(m_componentTypes.find(typeName) != m_componentTypes.end() && "Component not registered before use.");

	return std::static_pointer_cast<ComponentArray<T>>(m_componentArrays[typeName]);
}
