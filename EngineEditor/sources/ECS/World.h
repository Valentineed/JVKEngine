#pragma once
#include <memory>

#include "CommonTypes.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include "SystemManager.h"

#include "Vector/Vector3.h"


struct Actor;

class World
{
public:
	static World& GetInstance();
	
	void Init();

	//Entity
	Entity CreateEntity();

	void DestroyEntity(Entity entity);

	//Components
	template<typename T>
	void RegisterComponent();

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
	
	template<typename T>
	ComponentType GetComponentType();

	//System
	template<typename T>
	std::shared_ptr<T> RegisterSystem();

	template<typename T>
	void SetSystemSignature(Signature signature);

	template<typename T>
	T* GetSystem();

	template<typename T, typename...Args>
	void EmplaceComponent(Entity entity, Args&&... args);

	void CreateScriptingComponent(Entity entity, const std::string& string);
	void CreateCameraComponent(Entity entity, LibMath::Vector3 position, LibMath::Vector3 front, LibMath::Vector3 worldUp);
	void CreateMeshComponent(Entity entity, size_t indexTransform);
	void CreateSphereShapePhysicsComponent(Entity entity);
	void CreateCapsuleShapePhysicsComponent(Entity entity);
	void CreateBoxShapePhysicsComponent(Entity entity);
	
	void CreateDirectionalLightComponent(Entity entity);
	void CreateSpotlightComponent(Entity entity, LibMath::Vector3 position);
	void CreatePointlightComponent(Entity entity);
	
	void CreateSoundComponent(Entity entity);
	
	void SaveScene();


	void RemoveActor(Actor* actor);
	
	void AddActorToRoot(std::string name);

	void AddActorToActor(Actor* actor, std::string name);

	LibMath::Vector3 m_rightAxis = { 1.0f,0.f,0.f };
	LibMath::Vector3 m_up = { 0.f,1.0f,0.f };
	
private:
	std::unique_ptr<ComponentManager> m_componentManager;
	std::unique_ptr<EntityManager> m_entityManager;
	std::unique_ptr<SystemManager> m_systemManager;
	
private:
	/*Singleton*/
	World() = default;
	~World() = default;
	World(World const&) = delete;
	World& operator=(World const&) = delete;
};

//TODO deplace .inl ?
template<typename T>
inline void World::RegisterComponent()
{
	m_componentManager->RegisterComponent<T>();
}

template<typename T>
inline void World::AddComponent(Entity entity, T&& component)
{
	m_componentManager->AddComponent<T>(entity, std::move(component));

	auto signature = m_entityManager->GetSignature(entity);
	signature.set(m_componentManager->GetComponentType<T>(), true);
	m_entityManager->SetSignature(entity, signature);

	m_systemManager->EntitySignatureChanged(entity, signature);
}

template<typename T>
inline void World::AddComponentID(Entity entity, size_t index)
{
	m_componentManager->AddComponentID<T>(entity, index);

	auto signature = m_entityManager->GetSignature(entity);
	signature.set(m_componentManager->GetComponentType<T>(), true);
	m_entityManager->SetSignature(entity, signature);

	m_systemManager->EntitySignatureChanged(entity, signature);
}



template<typename T>
inline void World::RemoveComponent(Entity entity)
{
	m_componentManager->RemoveComponent<T>(entity);

	auto signature = m_entityManager->GetSignature(entity);
	signature.set(m_componentManager->GetComponentType<T>(), false);
	m_entityManager->SetSignature(entity, signature);

	m_systemManager->EntitySignatureChanged(entity, signature);
}


template<typename T>
inline void World::RemoveComponentID(Entity entity)
{
	m_componentManager->RemoveComponentID<T>(entity);

	auto signature = m_entityManager->GetSignature(entity);
	signature.set(m_componentManager->GetComponentType<T>(), false);
	m_entityManager->SetSignature(entity, signature);

	m_systemManager->EntitySignatureChanged(entity, signature);
}

template <typename T>
void World::ClearComponentID(Entity entity)
{
	m_componentManager->ClearComponentID<T>(entity);
	auto signature = m_entityManager->GetSignature(entity);
	signature.set(m_componentManager->GetComponentType<T>(), false);
	m_entityManager->SetSignature(entity, signature);

	m_systemManager->EntitySignatureChanged(entity, signature);
}

template<typename T>
inline T* World::GetComponent(Entity entity)
{
	return m_componentManager->GetComponent<T>(entity);
}

template<typename T>
inline size_t World::GetComponentID(Entity entity)
{
	return m_componentManager->GetComponentID<T>(entity);
}

template<typename T>
inline ComponentType World::GetComponentType()
{
	return m_componentManager->GetComponentType<T>();
}

template<typename T>
inline std::shared_ptr<T> World::RegisterSystem()
{
	return m_systemManager->RegisterSystem<T>();
}

template<typename T>
inline void World::SetSystemSignature(Signature signature)
{
	m_systemManager->SetSignature<T>(signature);
}

template<typename T>
inline T* World::GetSystem()
{
	return m_systemManager->GetSystem<T>();
}

template<typename T, typename ...Args>
inline void World::EmplaceComponent(Entity entity, Args&& ...args)
{
	m_componentManager->EmplaceComponent<T>(entity, std::forward<Args>(args)...);

	auto signature = m_entityManager->GetSignature(entity);
	signature.set(m_componentManager->GetComponentType<T>(), true);
	m_entityManager->SetSignature(entity, signature);

	m_systemManager->EntitySignatureChanged(entity, signature);
}
