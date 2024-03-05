#include "Trampoline.h"


#include "Actors/RigidType.h"
#include "../Systems/StaticMeshSystem.h"
#include "../Components/Transform.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/PhysixSystem.h"
#include "RenderingManager.h"
#include "Forces/ForceMode.h"
#include "Shapes/Primitives/BoxShape.h"

Trampoline::Trampoline()
{
	m_name = "Trampoline";

	const size_t indexMesh = World::GetInstance().GetSystem<StaticMeshSystem>()->RegisterDefault(GetTransform()->myIndexSystem, "resources/models/cube.obj", "resources/images/grass.jpg");

	AddComponent<StaticMesh>(indexMesh);
	
	BoxShape* shape = new BoxShape{ 0.5f,0.5f,0.5f };
	Collider col(*shape, RigidType::STATIC, *GetTransform(), m_idEntity);
	col.TriggerCollision(true);
	col.BindOnBeginContact(this, &Trampoline::OnBeginOverlap);
	
	const size_t indexCollider = World::GetInstance().GetSystem<PhysixSystem>()->Register(col);
	AddComponent<Collider>(indexCollider);
	
}

Trampoline::~Trampoline()
{
}

void Trampoline::Update(float deltaTime)
{
	Actor::Update(deltaTime);
	if(m_bApplyForce)
	{
		m_bApplyForce = false;
		ForceMode force(FORCE);
		m_box->GetPhysicsComponent()->AddForce({ 1000,1000,0 }, force);
	}
}

void Trampoline::Begin()
{
	Actor::Begin();
}

void Trampoline::End()
{
	Actor::End();
}


void Trampoline::OnBeginOverlap(size_t indexOtherCollider)
{
	auto* scene = SceneManager::GetInstance().GetScene();
	auto* otherActor = scene->GetActor(scene->m_root, indexOtherCollider);
	if(otherActor->m_name == "PickUpObject")
	{
		m_box = otherActor;
		m_bApplyForce = true;
	}
}
