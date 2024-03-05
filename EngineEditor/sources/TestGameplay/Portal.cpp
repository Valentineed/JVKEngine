#include "Portal.h"



#include "Capsule.h"
#include "../Scene/SceneManager.h"
#include "../Systems/PhysixSystem.h"
#include "../Systems/StaticMeshSystem.h"
#include "Actors/RigidType.h"
#include "Shapes/Primitives/BoxShape.h"


Portal::Portal(const Vector3& pos)
{
	auto* scene = SceneManager::GetInstance().GetScene();
	auto portal = scene->GetActorName(SceneManager::GetInstance().GetScene()->m_root, "Portal");
	if (portal)
	{
		portal->Remove();
	}
	m_name = "Portal";
	GetTransform()->SetLocalPosition(pos);
	size_t indexMesh = World::GetInstance().GetSystem<StaticMeshSystem>()->RegisterDefault(m_indexTransform, "resources/models/cube.obj", "Assets/StarterContent/red.png");
	AddComponent<StaticMesh>(indexMesh);

	BoxShape* shape = new BoxShape{ 0.25f,0.25f,0.25f };
	Collider col(*shape, RigidType::STATIC, *GetTransform(), m_idEntity);
	col.TriggerCollision(true);
	col.BindOnBeginContact(this, &Portal::BeginOverlap);
	col.BindOnEndContact(this, &Portal::EndOverlap);
	const size_t indexCollider = World::GetInstance().GetSystem<PhysixSystem>()->Register(col);

	AddComponent<Collider>(indexCollider);
	SceneManager::GetInstance().GetScene()->m_root->AddChild(this);
}

void Portal::BeginOverlap(size_t idOtherEntity)
{
	auto* scene = SceneManager::GetInstance().GetScene();
	auto* otherActor = scene->GetActor(scene->m_root, idOtherEntity);
	if (otherActor->m_name == "Capsule")
	{
		auto portal2 = scene->GetActorName(scene->m_root, "Portal2");
		if(portal2)
		{
			otherActor->GetTransform()->SetLocalPosition(portal2->GetTransform()->GetWorldLocation());
		}
	}
}

void Portal::EndOverlap(size_t idOtherEntity)
{
}
