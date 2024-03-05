#include "PickUpObject.h"

#include "Capsule.h"
#include "../Components/Transform.h"
#include "../Scene/SceneManager.h"
#include "../Systems/PhysixSystem.h"
#include "../Systems/StaticMeshSystem.h"
#include "../Systems/TransformSystem.h"
#include "Actors/RigidType.h"
#include "Shapes/Primitives/BoxShape.h"


PickUpObject::PickUpObject()
{
	m_name = "PickUpObject";

	const size_t indexMesh = World::GetInstance().GetSystem<StaticMeshSystem>()->RegisterDefault(m_indexTransform, "resources/models/cube.obj", "resources/images/cube.jpg");

	AddComponent<StaticMesh>(indexMesh);
	
	const size_t indexCollider = World::GetInstance().GetSystem<PhysixSystem>()->RegisterDefaultShape(BoxShape{ 0.5f,0.5f,0.5f }, RigidType::DYNAMIC, GetTransform(), m_idEntity);

	AddComponent<Collider>(indexCollider);
	GetPhysicsComponent()->BindOnBeginContact(this, &PickUpObject::BeginOverlap);
	GetPhysicsComponent()->BindOnEndContact(this, &PickUpObject::EndOverlap);

}

PickUpObject::~PickUpObject()
{
}

void PickUpObject::Update(float deltaTime)
{
	Actor::Update(deltaTime);
}

void PickUpObject::Begin()
{
	Actor::Begin();
	
}

void PickUpObject::End()
{
	Actor::End();
}

void PickUpObject::BeginOverlap(size_t idOtherEntity)
{
	auto* scene = SceneManager::GetInstance().GetScene();
	auto* otherActor = scene->GetActor(scene->m_root, idOtherEntity);
	if (otherActor->m_name == "Capsule")
	{
		Capsule* t = dynamic_cast<Capsule*>(otherActor);
		if(t)
		{
			t->m_pickObject = this;
		}
	}
	if(otherActor->m_name == "Trap")
	{
		indexIgnored = idOtherEntity;
	}
}

void PickUpObject::EndOverlap(size_t idOtherEntity)
{
	if(indexIgnored)
	{
		return;
	}
	auto* scene = SceneManager::GetInstance().GetScene();
	auto* otherActor = scene->GetActor(scene->m_root, idOtherEntity);
	if (otherActor->m_name == "Capsule")
	{
		Capsule* t = dynamic_cast<Capsule*>(otherActor);
		if (t && !t->m_bIsPick)
		{
			t->m_pickObject = nullptr;
		}
	}
}
