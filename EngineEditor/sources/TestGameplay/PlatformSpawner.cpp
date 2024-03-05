#include "PlatformSpawner.h"


#include "Bullet.h"
#include "Bullet2.h"
#include "Platform.h"
#include "../Components/Collider.h"
#include "../Components/Lights/SpotLight.h"
#include "../Scene/SceneManager.h"
#include "../Systems/PhysixSystem.h"
#include "../Systems/StaticMeshSystem.h"
#include "../Systems/TransformSystem.h"
#include "Shapes/Primitives/BoxShape.h"

PlatformSpawner::PlatformSpawner()
{
	m_name = "PlatformSpawner";

	size_t indexMesh = World::GetInstance().GetSystem<StaticMeshSystem>()->RegisterDefault(m_indexTransform, "resources/models/cube.obj", "resources/images/target.png");
	AddComponent<StaticMesh>(indexMesh);

	BoxShape* box = new BoxShape(1, 1, 1);
	Collider col(*box, RigidType::STATIC, *GetTransform(), m_idEntity);

	col.TriggerCollision(true);
	col.BindOnBeginContact(this, &PlatformSpawner::BeginOverlap);

	const size_t indexCollider = World::GetInstance().GetSystem<PhysixSystem>()->Register(col);
	AddComponent<Collider>(indexCollider);

	//SpotLight light(GetTransform()->GetLocalLocation(), GetTransform()->GetLocalRotation().EulerAngles(), 5.f, 10.f, { 0.7f,0.7f,0.7f }, { 0.7f,0.7f,0.7f }, { 0.7f,0.7f,0.7f }, 0.1f, 0.2f, 0.5f);
	//size_t indexLight = World::GetInstance().GetSystem<LightSystem>()->Register(light);
	//AddComponent<LightComponent>(indexLight);
	//World::GetInstance().GetSystem<LightSystem>()->GetLight<SpotLight>(indexLight)->SetPosition(Vector4(GetTransform()->GetLocalLocation().x, GetTransform()->GetLocalLocation().y, GetTransform()->GetLocalLocation().z, 1.f));
}

void PlatformSpawner::Update(float deltaTime)
{
	if(m_bSpawned && m_bFinished)
	{
		m_spawnedP = new Platform({ GetTransform()->GetLocalLocation().x ,GetTransform()->GetLocalLocation().y - 5, GetTransform()->GetLocalLocation().z });
		World::GetInstance().GetSystem<TransformSystem>()->GetTransform(m_spawnedP->GetMeshComponent()->transformID)->Scale(5, 0.2f, 5);
		m_spawnedP->GetTransform()->SetLocalPosition(GetTransform()->GetLocalLocation().x, GetTransform()->GetLocalLocation().y - 5, GetTransform()->GetLocalLocation().z);
		SceneManager::GetInstance().GetScene()->m_root->AddChild(m_spawnedP);
		m_bFinished = false;
	}
}

void PlatformSpawner::BeginOverlap(size_t idOtherEntity)
{
	auto* scene = SceneManager::GetInstance().GetScene();
	auto otherActor = scene->GetActor(scene->m_root, idOtherEntity);
	if (otherActor->m_name == "Bullet")
	{
		static_cast<Bullet*>(otherActor)->m_time = 10;
	}
	else if(otherActor->m_name == "Bullet2")
	{
		static_cast<Bullet2*>(otherActor)->m_bDestroy = true;
	}
	if (m_bSpawned)
		return;


	m_bSpawned = true;
	m_bFinished = true;
}