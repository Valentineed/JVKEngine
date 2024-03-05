#include "CustomActor.h"

#include "../Components/Transform.h"						//GetTransform
#include "../Systems/RenderSystem.h"						//GetSystem<RenderSystem>()
#include "../Systems/PhysixSystem.h"						//GetSystem<PhysixSystem>()
#include "../Systems/StaticMeshSystem.h"					//GetSystem<StaticMeshSystem>()

#include "RenderingManager.h"								//RenderingMgr functions
#include "Actors/RigidType.h"								//RigidType::DYNAMIC
#include "Shapes/ComplexMesh/ConvexMesh.h"					//ConvexMesh
#include "Shapes/ComplexMesh/ConvexMeshDescriptor.h"		//ConvexMeshDesc

CustomActor::CustomActor(std::string_view objPath, std::string_view texturePath)
{
	m_name = "CustomActor";
	
	size_t indexMesh = World::GetInstance().GetSystem<StaticMeshSystem>()->RegisterDefault(GetTransform()->myIndexSystem, objPath.data(), texturePath.data());
	AddComponent<StaticMesh>(indexMesh);
}

//CustomActor::~CustomActor()
//{
//	DeleteThis(false);
//}

void CustomActor::Update(float deltaTime)
{
	Actor::Update(deltaTime);
}

void CustomActor::Begin()
{
	Actor::Begin();
}

void CustomActor::End()
{
	Actor::End();
}