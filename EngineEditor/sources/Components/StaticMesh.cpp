#include "../ECS/World.h"
#include "../Systems/RenderSystem.h"
#include "RenderingManager.h"
#include "StaticMesh.h"

#include "../Systems/StaticMeshSystem.h"

StaticMesh::StaticMesh(size_t indexActorTransform, size_t indexMesh, std::string meshPath, std::string texturePath) :
myIndex(indexMesh), transformID(indexActorTransform)
{
	LoadModel(meshPath);
	LoadTexture(texturePath);
}

StaticMesh::~StaticMesh()
{
	if(bisRegistered)
	{
		World::GetInstance().GetSystem<StaticMeshSystem>()->RemoveStaticMesh(myIndex, -1);
	}
}

void StaticMesh::LoadModel(std::string path)
{
	RenderSystem* render = World::GetInstance().GetSystem<RenderSystem>();

	if (modelID != s_invalidIndex)
	{
		render->m_renderingMgr->ChangeModel(modelID, path);

		if (path.find("JVK_Engine\\") != std::string::npos)
		{
			auto count = path.find("JVK_Engine\\");
			path.erase(0, count + 11);
		}

		std::filesystem::path fileName = path;
		
		m_staticMeshName = fileName.filename().string();
		m_currentMeshPath = fileName.string();
		return;
	}

	modelID = render->m_renderingMgr->LoadModel(path);

	std::filesystem::path fileName = path;

	m_staticMeshName = fileName.filename().string();

	m_currentMeshPath = fileName.string();
}

void StaticMesh::LoadTexture(std::string path)
{
	//TODO add commit for user "Load first your Model!"
	if (modelID != s_invalidIndex)
	{

		RenderSystem* render = World::GetInstance().GetSystem<RenderSystem>();
		render->m_renderingMgr->AddTexture(path, modelID);

		if (path.find("JVK_Engine\\") != std::string::npos)
		{
			auto count = path.find("JVK_Engine\\");
			path.erase(0, count + 11);
		}
		
		std::filesystem::path fileName = path;

		m_staticMeshTextureName = fileName.filename().string();

		m_currentTexturePath = fileName.string();
	}
}

void StaticMesh::SetTransform(size_t indexTransform)
{
	World::GetInstance().GetSystem<StaticMeshSystem>()->m_staticMeshes[myIndex].transformID = indexTransform;
	transformID = indexTransform;
}
