#pragma once
#include <string_view>

struct StaticMesh
{
	StaticMesh() = default;
	StaticMesh(size_t indexActorTransform, size_t indexMesh, std::string meshPath, std::string texturePath);
	~StaticMesh();
	void LoadModel(std::string path);
	void LoadTexture(std::string path);
	void SetTransform(size_t indexTransform);
	size_t myIndex = s_invalidIndex;
	size_t modelID = s_invalidIndex;
	size_t textureID = s_invalidIndex;
	size_t transformID = s_invalidIndex;

	std::string m_staticMeshName;
	std::string m_staticMeshTextureName;

	std::string m_currentMeshPath;
	std::string m_currentTexturePath;
	//todo Material
protected:
	static  const  size_t s_invalidIndex = SIZE_MAX;
private:
	bool bisRegistered = false;
};
