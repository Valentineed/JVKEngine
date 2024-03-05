#include "Trap.h"


#include "../Systems/PhysixSystem.h"
#include "../Systems/StaticMeshSystem.h"
#include "Actors/RigidType.h"
#include "Shapes/Primitives/BoxShape.h"
#include "../Components/Transform.h"

Trap::Trap()
{
	m_name == "Trap";
	
	const size_t indexMesh = World::GetInstance().GetSystem<StaticMeshSystem>()->RegisterDefault(m_indexTransform, "resources/models/cube.obj", "Assets/StarterContent/red.png");

	AddComponent<StaticMesh>(indexMesh);

	const size_t indexCollider = World::GetInstance().GetSystem<PhysixSystem>()->RegisterDefaultShape(BoxShape{ 1.f,0.2f,1.f }, RigidType::STATIC, GetTransform(), m_idEntity);

	AddComponent<Collider>(indexCollider);
	GetTransform()->SetLocalPosition(-11, 9, 0);
	GetTransform()->SetScale({ 1.f,0.2f,1.f });
}
