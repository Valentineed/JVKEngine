#pragma once

#include <stack>
#include <vector>

#include "../Components/StaticMesh.h"
#include "../ECS/System.h"

class RenderingMgr;
class TransformSystem;

class StaticMeshSystem : public System
{
public:
	void Init(TransformSystem* transformSystem);
	void Update(RenderingMgr* renderingMgr);
	size_t Register(StaticMesh staticMesh);
	size_t RegisterDefault(size_t indexActorTransform, std::string meshPath = "resources/models/cube.obj", std::string texturePath = "resources/images/missing-texture.png");
	void RemoveStaticMesh(size_t index, int idEntity);
	std::vector<StaticMesh> m_staticMeshes;

	StaticMesh* GetStaticMesh(size_t index);

private:
	std::stack<size_t> m_freeIndex;
	TransformSystem* m_transformSystem = nullptr;
	const  size_t m_invalidIndex = SIZE_MAX;
};
