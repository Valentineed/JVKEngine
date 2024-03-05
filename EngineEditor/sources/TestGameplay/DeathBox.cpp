#include "DeathBox.h"

#include "../Components/Collider.h"
#include "../Scene/SceneManager.h"
#include "../Systems/PhysixSystem.h"
#include "Shapes/Primitives/BoxShape.h"

DeathBox::DeathBox()
{
	m_name = "DeathBox";
	m_scales.push_back({ 2,0.5,2 });
	m_scales.push_back({ 10,0.2,10 });
	m_scales.push_back({ 25,0.2,25 });
	const auto extends = m_scales[SceneManager::GetInstance().m_indexUpdate];
	BoxShape* box = new BoxShape(extends.x, extends.y, extends.z);
	Collider col(*box, RigidType::STATIC, *GetTransform(), m_idEntity);

	col.TriggerCollision(true);
	col.BindOnBeginContact(this, &DeathBox::BeginOverlap);

	const size_t indexCollider = World::GetInstance().GetSystem<PhysixSystem>()->Register(col);

	AddComponent<Collider>(indexCollider);
	m_positions.push_back({ 7,1,7 });
	m_positions.push_back({ 4,3,0 });
	m_positions.push_back({ 9,5,0 });
}

void DeathBox::BeginOverlap(size_t idOtherEntity)
{
	const auto root = SceneManager::GetInstance().GetScene()->m_root;
	const auto actor = SceneManager::GetInstance().GetScene()->GetActor(root, idOtherEntity);

	if (actor->m_name == "Capsule")
	{
		actor->GetTransform()->SetLocalPosition(m_positions[SceneManager::GetInstance().m_indexUpdate]);
	}
}