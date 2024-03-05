#include "LvlManager.h"

#include "../Components/Transform.h"
#include "../InputSystem/InputManager.h"
#include "../Scene/SceneManager.h"
#include "../Systems/StaticMeshSystem.h"
#include "../Systems/TransformSystem.h"
#include "../Systems/PhysixSystem.h"
#include "../Systems/RenderSystem.h"
#include "Shapes/Primitives/SphereShape.h"
#include "Actors/RigidType.h"
#include "Shapes/Primitives/BoxShape.h"
#include "RenderingManager.h"

LvlManager::LvlManager()
{
	m_name = "LvlManager";
	size_t indexMesh = World::GetInstance().GetSystem<StaticMeshSystem>()->RegisterDefault(m_indexTransform, "resources/models/sphere.fbx");

	AddComponent<StaticMesh>(indexMesh);

	BoxShape* box = new BoxShape(2, 2, 2);
	Collider col(*box, RigidType::STATIC, *GetTransform(), m_idEntity);

	col.TriggerCollision(true);
	col.BindOnBeginContact(this, &LvlManager::BeginOverlap);

	const size_t indexCollider = World::GetInstance().GetSystem<PhysixSystem>()->Register(col);

	AddComponent<Collider>(indexCollider);
	m_lvls.emplace_back("Level1");
	m_lvls.emplace_back("Level2");
	m_lvls.emplace_back("Level3");
	m_lvls.emplace_back("EndGame");
	m_timesToReset.emplace_back(33.f);
	m_timesToReset.emplace_back(23.f);
	m_timesToReset.emplace_back(53.f);

	
}

void LvlManager::Update(float deltaTime)
{
	Actor::Update(deltaTime);
	
	m_time += m_scale * deltaTime;

	auto* scene = &SceneManager::GetInstance();
	
	World::GetInstance().GetSystem<RenderSystem>()->GetViewportWindow().timer = m_timesToReset[scene->m_indexUpdate] - m_time;
	
	if (m_time >= m_timesToReset[scene->m_indexUpdate])
	{
		InputManager::GetInstance().Clear();
		scene->GetScene()->End();
		scene->LoadKnownScene(scene->GetKnownScenes().find(m_lvls[scene->m_indexUpdate])->second);
		scene->GetScene()->Begin();
		scene->GetScene()->BeginPlay();
	}
	else if(m_changeScene)
	{
		auto* scene = &SceneManager::GetInstance();
		InputManager::GetInstance().Clear();
		scene->GetScene()->End();
		scene->LoadKnownSceneOnUpdate(scene->GetKnownScenes().find(m_lvls[scene->m_indexUpdate + 1])->second);
		auto s = scene->GetScene();
		scene->GetScene()->Begin();
		scene->GetScene()->BeginPlay();
		m_changeScene = false;
	}
}

void LvlManager::Begin()
{
	Actor::Begin();
	auto* scene = &SceneManager::GetInstance();
	
	World::GetInstance().GetSystem<RenderSystem>()->GetViewportWindow().timer = m_timesToReset[scene->m_indexUpdate];
}

void LvlManager::End()
{
	Actor::End();
}

void LvlManager::BeginOverlap(size_t idOtherEntity)
{
	const auto root = SceneManager::GetInstance().GetScene()->m_root;
	const auto actor = SceneManager::GetInstance().GetScene()->GetActor(root, idOtherEntity);

	if (actor->m_name == "Capsule")
	{
		m_changeScene = true;
	}
}
