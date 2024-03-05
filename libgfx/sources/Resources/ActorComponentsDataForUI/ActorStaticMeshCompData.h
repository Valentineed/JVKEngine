#pragma once
#include <imgui_impl_glfw.h>
#include <string>
#include <libgfx_Export.h>

class ActorStaticMeshCompData
{
public:
	ActorStaticMeshCompData() = default;
	//Getters
	libgfx_API const std::string& GetStaticMeshName();
	libgfx_API const std::string& GetStaticMeshTextureName();
	libgfx_API const std::string& GetStaticMeshTexturePath();
	libgfx_API const std::string& GetNewMeshActorPath();
	libgfx_API const std::string& GetNewMeshTexturePath();

	libgfx_API ImTextureID& GetStaticMeshTextureID();

	libgfx_API bool IsMeshUpdated();
	libgfx_API bool IsMeshTextureUpdated();

	//Setters
	void libgfx_API SetStaticMeshName(std::string meshName);
	void libgfx_API SetStaticMeshTextureName(std::string meshTextureName);
	void libgfx_API SetStaticMeshTexturePath(std::string meshTexturePath);
	void libgfx_API SetNewStaticMeshPath(std::string meshPath);
	void libgfx_API SetNewStaticMeshTexturePath(std::string meshTexturePath);

	//Others
	void libgfx_API ChangeStaticMesh(std::string& pathGiven);
	void libgfx_API ChangeStaticMeshTexture(std::string& pathGiven);

	void libgfx_API UpdateMeshData(std::string& meshName);
	void libgfx_API UpdateTextureMeshData(std::string& meshTextureName);

private:
	std::string m_staticMeshName;
	std::string m_staticMeshTextureName;
	std::string m_staticMeshTexturePath;
	std::string m_newMeshActorPath;
	std::string m_newMeshTexturePath;
	
	ImTextureID m_staticMeshTextureID;

	bool m_bIsMeshUpdated = false;
	bool m_bIsTextureMeshUpdated = false;

	
};

