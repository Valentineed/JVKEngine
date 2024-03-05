#include "Button.h"



#include "Capsule.h"
#include "Trap.h"
#include "../Scene/SceneManager.h"
#include "../Systems/PhysixSystem.h"
#include "../Systems/StaticMeshSystem.h"
#include "Actors/RigidType.h"
#include "Shapes/Primitives/BoxShape.h"

Button::Button()
{
	m_name = "Button";
	const size_t indexMesh = World::GetInstance().GetSystem<StaticMeshSystem>()->RegisterDefault(m_indexTransform, "resources/models/cube.obj", "Assets/StarterContent/green.png");

	AddComponent<StaticMesh>(indexMesh);
	
	BoxShape* shape = new BoxShape{ 0.25f,0.25f,0.25f };
	Collider col(*shape, RigidType::STATIC, *GetTransform(), m_idEntity);
	col.TriggerCollision(true);
	col.BindOnBeginContact(this, &Button::OnBeginOverlap);
	col.BindOnEndContact(this, &Button::OnEndOverlap);
	const size_t indexCollider = World::GetInstance().GetSystem<PhysixSystem>()->Register(col);
	AddComponent<Collider>(indexCollider);

}

void Button::OnBeginOverlap(size_t idOtherActor)
{
	if(m_bFinished)
	{
		return;
	}
	auto* scene = SceneManager::GetInstance().GetScene();
	auto* otherActor = scene->GetActor(scene->m_root, idOtherActor);
	if (otherActor->m_name == "Capsule")
	{
		Capsule* t = dynamic_cast<Capsule*>(otherActor);
		if (t)
		{
			t->m_button = this;
			t->m_bOpenTrap = true;
		}
	}
}

void Button::OnEndOverlap(size_t idOtherActor)
{
	auto* scene = SceneManager::GetInstance().GetScene();
	auto* otherActor = scene->GetActor(scene->m_root, idOtherActor);
	if (otherActor->m_name == "Capsule")
	{
		Capsule* t = dynamic_cast<Capsule*>(otherActor);
		if (t)
		{
			t->m_bOpenTrap = false;
		}
	}
}

void Button::BeginPlay()
{
	Actor::BeginPlay();
	m_trap = new Trap{};
	SceneManager::GetInstance().GetScene()->m_root->AddChild(m_trap);
	if(SceneManager::GetInstance().m_indexUpdate == 2)
	{
		m_trap->GetTransform()->SetLocalPosition({-6,9,0});
	}
}

void Button::DeleteTrap()
{
	m_trap->Remove();
	m_bFinished = true;
	GetMeshComponent()->LoadTexture("Assets/StarterContent/red.png");
}
