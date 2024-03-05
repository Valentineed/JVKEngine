#pragma once
#include <cassert>
#include <memory>
#include <unordered_map>


#include "CommonTypes.h"
#include "System.h"
#include "Resources/HashString.h"
/*The System Manager is in charge of maintaining a record
 *of registered systems and their signatures*/
class SystemManager
{
public:
	template<typename T>
	std::shared_ptr<T> RegisterSystem();

	template<typename T>
	void SetSignature(Signature signature);


	template<typename T>
	T* GetSystem();
	
	void EntityDestroyed(Entity entity);

	void EntitySignatureChanged(Entity entity, Signature entitySignature);

private:
	std::unordered_map<size_t, Signature> m_signatures{};
	//Todo unique
	std::unordered_map<size_t, std::shared_ptr<System>> m_systems{};
};

//TODO deplace in .inl ?

template<typename T>
inline std::shared_ptr<T> SystemManager::RegisterSystem()
{
	size_t typeName = HashString::Hash(typeid(T).name());

	assert(m_systems.find(typeName) == m_systems.end() && "Registering system more than once.");

	auto system = std::make_shared<T>();
	m_systems.insert({ typeName, system });
	return system;
}

template<typename T>
inline void SystemManager::SetSignature(Signature signature)
{
	size_t typeName = HashString::Hash(typeid(T).name());

	assert(m_systems.find(typeName) != m_systems.end() && "System used before registered.");

	m_signatures.insert({ typeName, signature });
}

template<typename T>
inline T* SystemManager::GetSystem()
{
	size_t typeName = HashString::Hash(typeid(T).name());

	static_assert(std::is_base_of_v<System,T>, "GetSystem user pass invalid type");
	
	auto iterator = m_systems.find(typeName);
	if(iterator == m_systems.end())
	{
		return nullptr;
	}
	return (T*)m_systems.find(typeName)->second.get();
}
