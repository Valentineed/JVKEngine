#include "StaticMeshSystem.h"

#include "RenderingManager.h"
#include "RenderSystem.h"
#include "TransformSystem.h"

void StaticMeshSystem::Init(TransformSystem* transformSystem)
{
	if (!transformSystem)
	{
		throw std::runtime_error("transformSystem is null in staticMesh Init");
	}
	m_transformSystem = transformSystem;
}

void StaticMeshSystem::Update(RenderingMgr* renderingMgr)
{
	for (auto& staticMesh : m_staticMeshes)
	{
		if (staticMesh.modelID != m_invalidIndex && staticMesh.transformID != m_invalidIndex)
		{
			renderingMgr->SetTransform(0, staticMesh.modelID, m_transformSystem->GetTransform(staticMesh.transformID)->m_worldTransform);
		}
	}
}

size_t StaticMeshSystem::Register(StaticMesh staticMesh)
{
	if (m_freeIndex.empty())
	{
		size_t index = m_staticMeshes.size();
		m_staticMeshes.push_back(staticMesh);
		m_staticMeshes[index].myIndex = index;
		return  index;
	}
	const size_t index = m_freeIndex.top();
	m_freeIndex.pop();
	m_staticMeshes[index] = staticMesh;
	m_staticMeshes.back().myIndex = index;
	return  index;
}

size_t StaticMeshSystem::RegisterDefault(size_t indexActorTransform, std::string meshPath, std::string texturePath)
{
	if (m_freeIndex.empty())
	{
		size_t index = m_staticMeshes.size();
		m_staticMeshes.emplace_back(indexActorTransform, index, meshPath, texturePath);
		m_staticMeshes.back().SetTransform(indexActorTransform);
		m_staticMeshes.back().myIndex = index;
		return  index;
	}
	const size_t index = m_freeIndex.top();
	m_freeIndex.pop();
	m_staticMeshes[index] = { indexActorTransform, index, meshPath, texturePath };
	m_staticMeshes.back().myIndex = index;
	return  index;
}

void StaticMeshSystem::RemoveStaticMesh(size_t index, int idEntity)
{
	RenderSystem* render = World::GetInstance().GetSystem<RenderSystem>();

	render->m_renderingMgr->RemoveTexture(m_staticMeshes[index].modelID);
	render->m_renderingMgr->RemoveModel(m_staticMeshes[index].modelID);

	if (index != m_staticMeshes.size() - 1)
	{
		m_staticMeshes.back().modelID = m_staticMeshes[index].modelID;

		std::iter_swap(m_staticMeshes.begin() + index, m_staticMeshes.end() - 1);
	}
	m_staticMeshes.pop_back();

	if (idEntity != -1)
	{
		World::GetInstance().RemoveComponentID<StaticMesh>(idEntity);
	}
}

StaticMesh* StaticMeshSystem::GetStaticMesh(size_t index)
{
	return &m_staticMeshes[index];
}