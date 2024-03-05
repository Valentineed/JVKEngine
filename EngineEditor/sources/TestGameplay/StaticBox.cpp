#include "StaticBox.h"


#include "LvlManager.h"
#include "../Components/Transform.h"
#include "../Scene/SceneManager.h"
#include "../Systems/StaticMeshSystem.h"
#include "../Systems/TransformSystem.h"
#include "../Systems/PhysixSystem.h"
#include "Actors/RigidType.h"
#include "Shapes/Primitives/BoxShape.h"

StaticBox::StaticBox()
{
	m_name = "StaticBox";

	size_t indexMesh = World::GetInstance().GetSystem<StaticMeshSystem>()->RegisterDefault(m_indexTransform, "resources/models/cube.obj", "Assets/StarterContent/red.png");

	AddComponent<StaticMesh>(indexMesh);

	BoxShape* shape = new BoxShape{ 0.25f,0.25f,0.25f };
	Collider col(*shape, RigidType::STATIC, *GetTransform(), m_idEntity);
	col.TriggerCollision(true);
	col.BindOnBeginContact(this, &StaticBox::BeginOverlap);
	col.BindOnEndContact(this, &StaticBox::EndOverlap);
	const size_t indexCollider = World::GetInstance().GetSystem<PhysixSystem>()->Register(col);
	
	AddComponent<Collider>(indexCollider);
	
}

StaticBox::~StaticBox()
{
	//delete m_physixShape;
	//delete m_collider;
}

void StaticBox::Update(float deltaTime)
{
	Actor::Update(deltaTime);
}

void StaticBox::Begin()
{
	Actor::Begin();	
}

void StaticBox::End()
{
	Actor::End();

}

void StaticBox::BeginOverlap(size_t idOtherEntity)
{
	auto* scene = SceneManager::GetInstance().GetScene();
	if (scene->GetActor(scene->m_root, idOtherEntity)->m_name == "PickUpObject")
	{
		GetMeshComponent()->LoadTexture("Assets/StarterContent/green.png");
		scene->GetActor(scene->m_root, idOtherEntity)->GetTransform()->SetLocalRotation({ 0,0,0,1 });
		auto lvl = scene->GetActorName(scene->m_root, "LvlManager");
		if(lvl)
		{
			auto offsetPos = GetTransform()->GetWorldLocation();
			offsetPos.x += 8;
			static_cast<LvlManager*>(lvl)->GetTransform()->SetLocalPosition(offsetPos);
		}
	}
}

void StaticBox::EndOverlap(size_t idOtherEntity)
{
	auto* scene = SceneManager::GetInstance().GetScene();
	if (scene->GetActor(scene->m_root, idOtherEntity)->m_name == "PickUpObject")
	{
		GetMeshComponent()->LoadTexture("Assets/StarterContent/red.png");
		auto lvl = scene->GetActorName(scene->m_root, "LvlManager");
		if (lvl)
		{
			auto offsetPos = GetTransform()->GetWorldLocation();
			offsetPos.y += 100;
			static_cast<LvlManager*>(lvl)->GetTransform()->SetLocalPosition(offsetPos);
		}
	}
}
