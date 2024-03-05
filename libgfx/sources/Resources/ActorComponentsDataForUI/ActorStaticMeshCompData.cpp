#include "ActorStaticMeshCompData.h"

#include <windows.h>

libgfx_API const std::string& ActorStaticMeshCompData::GetStaticMeshName()
{
	return m_staticMeshName;
}

libgfx_API const std::string& ActorStaticMeshCompData::GetStaticMeshTextureName()
{
	return m_staticMeshTextureName;
}

libgfx_API const std::string& ActorStaticMeshCompData::GetStaticMeshTexturePath()
{
	return m_staticMeshTexturePath;
}

libgfx_API const std::string& ActorStaticMeshCompData::GetNewMeshActorPath()
{
	return m_newMeshActorPath;
}

libgfx_API const std::string& ActorStaticMeshCompData::GetNewMeshTexturePath()
{
	return m_newMeshTexturePath;
}

libgfx_API ImTextureID& ActorStaticMeshCompData::GetStaticMeshTextureID()
{
	return m_staticMeshTextureID;
}

libgfx_API bool ActorStaticMeshCompData::IsMeshUpdated()
{
	return m_bIsMeshUpdated;
}

libgfx_API bool ActorStaticMeshCompData::IsMeshTextureUpdated()
{
	return m_bIsTextureMeshUpdated;
}

void libgfx_API ActorStaticMeshCompData::SetStaticMeshName(std::string meshName)
{
	m_staticMeshName = meshName;
}

void libgfx_API ActorStaticMeshCompData::SetStaticMeshTextureName(std::string meshTextureName)
{
	m_staticMeshTextureName = meshTextureName;
}

void libgfx_API ActorStaticMeshCompData::SetStaticMeshTexturePath(std::string meshTexturePath)
{
	m_staticMeshTexturePath = meshTexturePath;
}

void libgfx_API ActorStaticMeshCompData::SetNewStaticMeshPath(std::string meshPath)
{
	m_newMeshActorPath = meshPath;
}

void libgfx_API ActorStaticMeshCompData::SetNewStaticMeshTexturePath(std::string meshTexturePath)
{
	m_newMeshTexturePath = meshTexturePath;
}

void libgfx_API ActorStaticMeshCompData::ChangeStaticMesh(std::string& pathGiven)
{
	m_newMeshActorPath = pathGiven;

	if (m_newMeshActorPath.find(".obj") != std::string::npos || m_newMeshActorPath.find(".fbx") != std::string::npos)
	{
		m_bIsMeshUpdated = true;
	}
	else
	{
		m_newMeshActorPath.clear();
		MessageBox(NULL, "The path given was not an obj or fbx file", "Error changing new model", MB_OK);
	}
}

void libgfx_API ActorStaticMeshCompData::ChangeStaticMeshTexture(std::string& pathGiven)
{
	m_newMeshTexturePath = pathGiven;

	if (m_newMeshTexturePath.find(".png") != std::string::npos || m_newMeshTexturePath.find(".jpg") != std::string::npos)
	{
		m_bIsTextureMeshUpdated = true;
	}
	else
	{
		m_newMeshTexturePath.clear();
		MessageBox(NULL, "The path given was not an png or jpg file", "Error changing new texture", MB_OK);
	}
}

void libgfx_API ActorStaticMeshCompData::UpdateMeshData(std::string& meshName)
{
	m_staticMeshName = meshName;

	m_newMeshActorPath.clear();
	m_bIsMeshUpdated = false;
}

void libgfx_API ActorStaticMeshCompData::UpdateTextureMeshData(std::string& meshTextureName)
{
	m_staticMeshTextureName = meshTextureName;

	m_newMeshTexturePath.clear();
	m_bIsTextureMeshUpdated = false;
}
