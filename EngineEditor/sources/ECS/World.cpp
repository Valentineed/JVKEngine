#include "World.h"


#include "../Components/Camera.h"
#include "../Components/Transform.h"
#include "../Components/ScriptComponent.h"
#include "../Components/StaticMesh.h"
#include "../Components/Lights/DirectionalLight.h"
#include "../Components/Lights/PointLight.h"
#include "../Components/Lights/SpotLight.h"
#include "../Systems/CameraSystem.h"
#include "../Systems/LightSystem.h"
#include "../Systems/PhysixSystem.h"
#include "../Systems/ScriptingSystem.h"
#include "../Systems/StaticMeshSystem.h"
#include "../Systems/SoundSystem.h"
#include "Shapes/Primitives/BoxShape.h"
#include "Shapes/Primitives/CapsuleShape.h"
#include "Shapes/Primitives/SphereShape.h"
#include "../Scene/Actor.h"
#include "../Scene/SceneManager.h"
#include "../Serialization/Serializer.h"


World& World::GetInstance()
{
	static World instance;
	
	return instance;
}

void World::Init()
{
	// Create pointers to each manager
	m_componentManager = std::make_unique<ComponentManager>();
	m_entityManager = std::make_unique<EntityManager>();
	m_systemManager = std::make_unique<SystemManager>();
}

Entity World::CreateEntity()
{
	return m_entityManager->CreateEntity();
}

void World::DestroyEntity(Entity entity)
{
	m_entityManager->DestroyEntity(entity);

	m_componentManager->EntityDestroyed(entity);

	m_systemManager->EntityDestroyed(entity);
}

void World::CreateScriptingComponent(Entity entity, const std::string& string)
{
	GetSystem<ScriptingSystem>()->Register(entity, string);
}

void World::CreateCameraComponent(Entity entity, Vector3 position, Vector3 front, Vector3 worldUp)
{
	size_t indexCamera = GetSystem<CameraSystem>()->Register(position, front, worldUp);

	AddComponentID<Camera>(entity, indexCamera);
}

void World::CreateMeshComponent(Entity entity, size_t indexTransform)
{
	size_t indexMesh = GetSystem<StaticMeshSystem>()->RegisterDefault(indexTransform);

	AddComponentID<StaticMesh>(entity, indexMesh);
}

void World::CreateSphereShapePhysicsComponent(Entity entity)
{
	auto actor = SceneManager::GetInstance().GetScene()->GetActor(SceneManager::GetInstance().GetScene()->m_root, entity);
	size_t indexCollider = GetSystem<PhysixSystem>()->RegisterDefaultShape(SphereShape{ 1.f }, RigidType::DYNAMIC, actor->GetTransform(), entity);

	AddComponentID<Collider>(entity, indexCollider);
}

void World::CreateCapsuleShapePhysicsComponent(Entity entity)
{
	auto actor = SceneManager::GetInstance().GetScene()->GetActor(SceneManager::GetInstance().GetScene()->m_root, entity);
	size_t indexCollider =GetSystem<PhysixSystem>()->RegisterDefaultShape(CapsuleShape{ 1.f, 0.5f }, RigidType::DYNAMIC, actor->GetTransform(), entity);

	AddComponentID<Collider>(entity, indexCollider);
}

void World::CreateBoxShapePhysicsComponent(Entity entity)
{
	auto actor = SceneManager::GetInstance().GetScene()->GetActor(SceneManager::GetInstance().GetScene()->m_root, entity);
	size_t indexCollider = GetSystem<PhysixSystem>()->RegisterDefaultShape(BoxShape{ 0.5f,0.5f,0.5f }, RigidType::DYNAMIC, actor->GetTransform(), entity);

	AddComponentID<Collider>(entity, indexCollider);
}

void World::CreateDirectionalLightComponent(Entity entity)
{
	DirectionalLight dirLight({ 0,1,0 }, { 0.7f,0.7f,0.7f }, { 0.7f,0.7f,0.7f }, { 0.7f,0.7f,0.7f });
	
	size_t indexLight = GetSystem<LightSystem>()->Register(dirLight);

	AddComponentID<LightComponent>(entity, indexLight);
}

void World::CreateSpotlightComponent(Entity entity, Vector3 position)
{
	SpotLight spoLight(position, { 0,-1,0 }, 12.5f, 17.5f, { 0.1f,0.1f,0.1f }, { 0.8f,0.8f,0.8f }, { 1.f,1.f,1.f }, 1.f, 0.09f, 0.032f);

	size_t indexLight = GetSystem<LightSystem>()->Register(spoLight);

	AddComponentID<LightComponent>(entity, indexLight);
}

void World::CreatePointlightComponent(Entity entity)
{
	PointLight pointLight({ 0,1,0 }, { 1,1,1 }, { 1,0,0 }, { 0,1,0 }, 0.2f, 0.5f, 0.5f);

	size_t indexLight = GetSystem<LightSystem>()->Register(pointLight);

	AddComponentID<LightComponent>(entity, indexLight);
}

void World::CreateSoundComponent(Entity entity)
{
	size_t indexSound3D = GetSystem<SoundSystem>()->Register("resources/sounds/in-the-dark.wav", { 0,0,0 }, 10, 5);
	AddComponentID<SoundComponent>(entity, indexSound3D);
}

void World::SaveScene()
{
	XML::Serializer::GetInstance().ExportScene(SceneManager::GetInstance().GetScene());
}

void World::RemoveActor(Actor* actor)
{
	actor->Remove();
}

void World::AddActorToRoot(std::string name)
{
	SceneManager::GetInstance().GetScene()->m_root->AddChild(new Actor(name));
}

void World::AddActorToActor(Actor* actor, std::string name)
{
	SceneManager::GetInstance().GetScene()->GetActor(SceneManager::GetInstance().GetScene()->m_root, actor->m_idEntity)->AddChild(new Actor(name));
}

