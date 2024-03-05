#include "Floor.h"
#include "../Systems/PhysixSystem.h"
#include "../Components/Collider.h"
#include "../Components/Transform.h"
#include "../Systems/StaticMeshSystem.h"
#include "../Components/Lights/DirectionalLight.h"


Floor::Floor()
{
	m_name = "Floor";


	size_t indexMesh = World::GetInstance().GetSystem<StaticMeshSystem>()->RegisterDefault(m_indexTransform, "Assets/StarterContent/Floor.obj");

	AddComponent<StaticMesh>(indexMesh);
	
	Collider colision(0, 1, 0, 0,*GetTransform(), m_idEntity);
	const size_t indexCollider = World::GetInstance().GetSystem<PhysixSystem>()->Register(colision);
	AddComponent<Collider>(indexCollider);

	DirectionalLight dirLight({ 0,1,0 }, { 0.7f,0.7f,0.7f }, { 0.7f,0.7f,0.7f }, { 0.7f,0.7f,0.7f });

	const size_t indexLight = World::GetInstance().GetSystem<LightSystem>()->Register(dirLight);
	
	AddComponent<LightComponent>(indexLight);
	
}
