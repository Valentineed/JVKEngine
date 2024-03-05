#include "Bullet.h"


#include "../Systems/PhysixSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/StaticMeshSystem.h"
#include "Actors/RigidType.h"

#include <Shapes\Primitives\SphereShape.h>


#include "Portal.h"
#include "../Scene/SceneManager.h"

Bullet::Bullet(const Vector3& velocity,const Vector3& pos, Actor* actor )
{
	auto* scene = SceneManager::GetInstance().GetScene();
	auto bullet2 = scene->GetActorName(SceneManager::GetInstance().GetScene()->m_root, "Bullet");
	if (bullet2)
	{
		bullet2->Remove();
	}
	m_name = "Bullet";

	GetTransform()->SetLocalPosition(pos);

	GetTransform()->SetScale({ 0.2f,0.2f,0.2f });
	const size_t indexMesh = World::GetInstance().GetSystem<StaticMeshSystem>()->RegisterDefault(m_indexTransform, "Assets/StarterContent/bullet.fbx", "Assets/StarterContent/red.png");

	AddComponent<StaticMesh>(indexMesh);


	Collider col(SphereShape{ 0.2f }, RigidType::DYNAMIC, pos, {}, m_idEntity);

	col.DisableGravity(true);

	col.SetLinearVelocity(velocity);

	col.BindOnBeginContact(this, &Bullet::BeginOverlap);
	
	const size_t indexCollider = World::GetInstance().GetSystem<PhysixSystem>()->Register(col);

	AddComponent<Collider>(indexCollider);

	SceneManager::GetInstance().GetScene()->m_root->AddChild(this);

	//GetPhysicsComponent()->SetAngularDamping(0.5f);

	//GetPhysicsComponent()->SetLinearVelocity(camFront * velocity);

	//GetPhysicsComponent()->BindOnBeginContact(this, &Bullet::BeginOverlap);
}

void Bullet::Update(float deltaTime)
{

	Actor::Update(deltaTime);

	GetTransform()->SetScale({ 0.2f,0.2f,0.2f });
	m_time += m_scale * deltaTime;

	if(m_time >= m_timeToDestroy)
	{
		Destroy();
	}
	else if (m_createPortal)
	{
		new Portal(GetTransform()->GetWorldLocation());
		m_createPortal = false;
		Destroy();
	}
}

void Bullet::End()
{
	Actor::End();
	Destroy();
}

void Bullet::BeginOverlap(size_t idOtherEntity)
{
	//m_time = m_timeToDestroy;
	auto* scene = SceneManager::GetInstance().GetScene();
	if (scene->GetActor(scene->m_root, idOtherEntity)->m_name != "PlatformSpawner")
	{
		m_createPortal = true;
	}
	else
	{
		m_time = m_timeToDestroy;
	}
}

void Bullet::Destroy()
{
	Remove();
}
