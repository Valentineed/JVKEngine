#include "Bullet2.h"



#include "Bullet.h"
#include "../Scene/SceneManager.h"
#include "../Systems/PhysixSystem.h"
#include "../Systems/StaticMeshSystem.h"
#include "Actors/RigidType.h"
#include "Forces/ForceMode.h"
#include "Shapes/Primitives/SphereShape.h"
#include "Portal2.h"

Bullet2::Bullet2(const Vector3& velocity, const Vector3& pos, Actor* actor)
{
	auto* scene = SceneManager::GetInstance().GetScene();
	auto bullet2 = scene->GetActorName(SceneManager::GetInstance().GetScene()->m_root, "Bullet2");
	if (bullet2)
	{
		bullet2->Remove();
	}
	m_name = "Bullet2";

	GetTransform()->SetLocalPosition(pos);

	GetTransform()->SetScale({ 0.2f,0.2f,0.2f });
	const size_t indexMesh = World::GetInstance().GetSystem<StaticMeshSystem>()->RegisterDefault(m_indexTransform, "Assets/StarterContent/bullet.fbx", "Assets/StarterContent/blue.png");

	AddComponent<StaticMesh>(indexMesh);


	Collider col(SphereShape{ 0.2f }, RigidType::DYNAMIC, pos, {}, m_idEntity);

	//col.DisableGravity(true);

	col.SetLinearVelocity(velocity);
	//ForceMode force(FORCE);
	//col.AddForce(velocity, force);
	//
	col.BindOnBeginContact(this, &Bullet2::BeginOverlap);

	//Vector3 test = { 0,0,0 };
	//Vector3 test2 = { 0,5,0 };
	//col.SetMassSpaceInertiaTensor(test);
	//col.SetMass();
	//col.SetAngularVelocity(test2);
	const size_t indexCollider = World::GetInstance().GetSystem<PhysixSystem>()->Register(col);

	AddComponent<Collider>(indexCollider);

	SceneManager::GetInstance().GetScene()->m_root->AddChild(this);
}

void Bullet2::Update(float deltaTime)
{
	Actor::Update(deltaTime);
	GetTransform()->SetScale({ 0.2f,0.2f,0.2f });
	if (m_changeVelocity)
	{
		GetPhysicsComponent()->SetLinearVelocity(-GetPhysicsComponent()->GetLinearVelocity());
		m_changeVelocity = false;
	}
	else if(m_createPortal && !m_changeVelocity)
	{
		new Portal2(GetTransform()->GetWorldLocation());
		m_createPortal = false;
		Destroy();
	}
	else if(m_bDestroy)
	{
		Destroy();
	}
}

void Bullet2::End()
{
}

void Bullet2::BeginOverlap(size_t idOtherEntity)
{
	//m_time = m_timeToDestroy;
	//GetPhysicsComponent()->GetLinearVelocity();
	auto* scene = SceneManager::GetInstance().GetScene();
	if (scene->GetActor(scene->m_root, idOtherEntity)->m_name == "PlatformSpawner")
	{
		m_bDestroy = true;
	}
	else
	{
		if (m_count == 1)
		{
			m_createPortal = true;
		}
		else if (m_count == 0)
		{
			m_changeVelocity = true;
			m_count++;
		}
	}
}

void Bullet2::Destroy()
{
	Remove();
}
