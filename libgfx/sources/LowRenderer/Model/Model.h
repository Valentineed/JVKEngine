#pragma once

#include "Mesh.h"
#include "Resources/SavedData.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"

#define DEFAULT_TEXTURE_PATH "resources/images/missing-texture.png"

struct DescriptorSetLayout;

namespace LowRenderer
{
	class Camera;
}

class Model : public SavedResource
{
public:
	Model() = default;
	libgfx_API Model(const char* path, bool bMultiThreaded = true);

	std::vector<Mesh> m_targetMeshes;
	Matrix4 m_modelMat = Matrix4::Identity();
	DataBuffers m_uniformBuffer;

	size_t m_indexDeviceMemory;
	size_t m_matID = static_cast<size_t>(-1);
	size_t m_meshID = static_cast<size_t>(-1);
	size_t m_setID = static_cast<size_t>(-1);

	static VkDeviceSize m_alignedSize;

	static void ProcessAlignedSize();

	void UploadData();
	void UploadBuffer(size_t currImg);

	void Draw(VkCommandBuffer cmdBuffer, VkPipelineLayout& pipelineLayout, VkDescriptorSet set);

	void CreateDescriptorSet(DescriptorSetLayout& setLayout);
	void UpdateDescriptorSet();

	void CreateDefault();
	void CreateModel(std::string_view pathObj);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* /*scene*/);
	void ProcessNode(aiNode* node, const aiScene* scene);

	void CleanupModel();
};

struct SavedModel
{
	Model model;
	size_t ID;

	SavedModel() = delete;
	SavedModel(const Model& mod, size_t id) :
		model(mod), ID(id)
	{
	}

	void Destroy()
	{
		model.CleanupModel();
	}
};