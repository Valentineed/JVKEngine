#pragma once
#include "../Components/StaticMesh.h"
#include "../Scene/Actor.h"
#include "Shapes/ComplexMesh/TriangleMeshDescriptor.h"

class Floor;
struct Collider;
class TriangleMesh;

class Mountain : public Actor
{
public:
	Mountain();
	~Mountain();
	
	void Update(float deltaTime) override;
	void Begin() override;
	void End() override;

private:
	Floor* m_floor;
	//StaticMesh m_mesh;
	
	//std::pair<std::vector<uint32_t>, std::vector<LibMath::Vector3>> m_dataObj;
	/*TriangleMesh* m_physixShape = nullptr;
	TriangleMeshDescriptor m_tDescriptor;
	Collider* m_collider;*/
	
};
