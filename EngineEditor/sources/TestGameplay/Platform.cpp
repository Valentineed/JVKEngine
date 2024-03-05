#include "Platform.h"

#include "../Components/Transform.h"
#include "../Systems/StaticMeshSystem.h"
#include "../Systems/TransformSystem.h"
#include "../Systems/PhysixSystem.h"
#include "Actors/RigidType.h"
#include "Shapes/Primitives/BoxShape.h"

Platform::Platform()
{
	m_name = "Platform";

	size_t indexMesh = World::GetInstance().GetSystem<StaticMeshSystem>()->RegisterDefault(m_indexTransform);
	AddComponent<StaticMesh>(indexMesh);

	const size_t indexCollider = World::GetInstance().GetSystem<PhysixSystem>()->RegisterDefaultShape(BoxShape{ 5.f, 0.2f, 5.f }, RigidType::STATIC, GetTransform(), m_idEntity);
	AddComponent<Collider>(indexCollider);
}

Platform::Platform(const Vector3& position)
{
	m_name = "Platform";
	GetTransform()->SetLocalPosition(position);

	size_t indexMesh = World::GetInstance().GetSystem<StaticMeshSystem>()->RegisterDefault(m_indexTransform, "resources/models/cube.obj", "resources/images/tile.png");
	AddComponent<StaticMesh>(indexMesh);

	const size_t indexCollider = World::GetInstance().GetSystem<PhysixSystem>()->RegisterDefaultShape(BoxShape{ 5.f, 0.2f, 5.f }, RigidType::STATIC, GetTransform(), m_idEntity);
	AddComponent<Collider>(indexCollider);
}

void Platform::Update(float deltaTime)
{
	Actor::Update(deltaTime);
}

void Platform::Begin()
{
	Actor::Begin();
}

void Platform::End()
{
	Actor::End();
	Remove();
}