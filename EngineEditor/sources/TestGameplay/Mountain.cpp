#include "Mountain.h"


#include "Floor.h"
#include "RenderingManager.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/TransformSystem.h"
#include "../Systems/PhysixSystem.h"
#include "Actors/RigidType.h"
#include "Shapes/ComplexMesh/TriangleMesh.h"

Mountain::Mountain()
{
	m_name = "Ground";

	//size_t indexMesh = World::GetInstance().GetSystem<StaticMeshSystem>()->RegisterDefault(m_indexTransform, "Assets/StarterContent/Floor.obj");

	//AddComponent<StaticMesh>(indexMesh);

	/*if (GetMeshComponent() != nullptr)*/
	//{
	//	std::pair<std::vector<uint32_t>, std::vector<LibMath::Vector3>> dataObj = World::GetInstance().GetSystem<RenderSystem>()->m_renderingMgr->GetModelVertex(GetMeshComponent()->modelID);
	//	TriangleMeshDescriptor tMeshDescriptor = TriangleMeshDescriptor{ dataObj.first, dataObj.second };

	//	TriangleMesh* triangleMesh = new TriangleMesh{ tMeshDescriptor };

	//	size_t indexCollider = World::GetInstance().GetSystem<PhysixSystem>()->RegisterDefaultTriangleMesh(*triangleMesh, RigidType::STATIC, GetTransform(),m_idEntity);

	//	AddComponent<Collider>(indexCollider);
	//}


	Collider colision(0, 1, 0, 0, *GetTransform(), m_idEntity);
	const size_t indexCollider = World::GetInstance().GetSystem<PhysixSystem>()->Register(colision);
	AddComponent<Collider>(indexCollider);
	

	//m_floor = new Floor();
	//SceneManager::GetInstance().GetScene()->m_root->AddChild(m_floor);
	/*m_dataObj = World::GetInstance().GetSystem<RenderSystem>()->m_renderingMgr->GetModelVertex(m_mesh.modelID);
	m_tDescriptor = TriangleMeshDescriptor{ m_dataObj.first, m_dataObj.second };
	m_physixShape = new TriangleMesh{ m_tDescriptor };

	m_collider = new Collider{ *m_physixShape, RigidType::STATIC, m_indexTranform };
	AddComponent(*m_collider);*/
}

Mountain::~Mountain()
{
	//delete m_floor;
	/*delete m_physixShape;
	delete m_collider;*/
}

void Mountain::Update(float deltaTime)
{
	Actor::Update(deltaTime);
}

void Mountain::Begin()
{
	Actor::Begin();


}

void Mountain::End()
{
	Actor::End();
}