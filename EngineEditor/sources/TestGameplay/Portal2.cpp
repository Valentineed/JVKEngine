#include "Portal2.h"

#include "../Components/Transform.h"
#include "../Scene/SceneManager.h"
#include "../Systems/PhysixSystem.h"
#include "../Systems/StaticMeshSystem.h"
#include "Actors/RigidType.h"
#include "Shapes/Primitives/BoxShape.h"

Portal2::Portal2(const Vector3& pos)
{
	auto* scene = SceneManager::GetInstance().GetScene();
	auto portal = scene->GetActorName(SceneManager::GetInstance().GetScene()->m_root, "Portal2");
	if (portal)
	{
		portal->Remove();
	}
	m_name = "Portal2";
	GetTransform()->SetLocalPosition(pos);
	size_t indexMesh = World::GetInstance().GetSystem<StaticMeshSystem>()->RegisterDefault(m_indexTransform, "resources/models/cube.obj", "Assets/StarterContent/blue.png");
	AddComponent<StaticMesh>(indexMesh);

	BoxShape* shape = new BoxShape{ 0.25f,0.25f,0.25f };
	Collider col(*shape, RigidType::STATIC, *GetTransform(), m_idEntity);
	col.TriggerCollision(true);
	col.BindOnBeginContact(this, &Portal2::BeginOverlap);
	col.BindOnEndContact(this, &Portal2::EndOverlap);
	const size_t indexCollider = World::GetInstance().GetSystem<PhysixSystem>()->Register(col);

	AddComponent<Collider>(indexCollider);
	SceneManager::GetInstance().GetScene()->m_root->AddChild(this);
}

void Portal2::BeginOverlap(size_t idOtherEntity)
{
	auto* scene = SceneManager::GetInstance().GetScene();
	auto* otherActor = scene->GetActor(scene->m_root, idOtherEntity);
	if (otherActor->m_name == "Capsule")
	{
		auto portal = scene->GetActorName(scene->m_root, "Portal");
		if (portal)
		{
			otherActor->GetTransform()->SetLocalPosition(portal->GetTransform()->GetWorldLocation());
		}
	}
}

void Portal2::EndOverlap(size_t idOtherEntity)
{
}
