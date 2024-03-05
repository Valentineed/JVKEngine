#include "Model.h"
#include "Descriptors.h"
#include "assimp/postprocess.h"
#include "Core/SwapChain.h"
VkDeviceSize Model::m_alignedSize = 0;

Model::Model(const char* path, bool bMultiThreaded) :
	Model()
{
	(void)bMultiThreaded;
	CreateModel(path);
	UploadData();
}

void Model::Draw(VkCommandBuffer cmdBuffer, VkPipelineLayout& pipelineLayout, VkDescriptorSet set)
{
	for (auto& targetMesh : m_targetMeshes)
	{
		targetMesh.Draw(cmdBuffer, pipelineLayout, set);
	}
}

void Model::CreateDescriptorSet(DescriptorSetLayout& setLayout)
{
	GraphicAPI* api = GraphicAPI::GetInstance();

	const size_t swapChainImageCount = api->GetSwapChain()->ImageCount();
	std::vector<VkDescriptorSetLayout> layouts(swapChainImageCount, setLayout.setLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = api->GetDescriptorPool();	//If using Pool of pool method, need to dynamically swap descpPool
	allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImageCount);
	allocInfo.pSetLayouts = layouts.data();


	if(api->MutFreeIDModelDescriptorSets().empty())
	{
		std::vector<VkDescriptorSet>* NewSet = __nullptr;
		NewSet = &api->MutDescriptorSets().emplace_back();
		m_setID = api->GetDescriptorSets().size() - 1;
		NewSet->resize(swapChainImageCount);

		if (vkAllocateDescriptorSets(api->GetDevice(), &allocInfo, (*NewSet).data()) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate descriptor sets!");
		}
	}
	else
	{
		m_setID = api->MutFreeIDModelDescriptorSets().top();
		api->MutFreeIDModelDescriptorSets().pop();
	}
}

void Model::UpdateDescriptorSet()
{
	GraphicAPI* api = GraphicAPI::GetInstance();

	for (size_t iSet = 0; iSet < api->GetDescriptorSets()[m_setID].size(); iSet++)
	{
		std::vector<VkWriteDescriptorSet> descriptorWrites;

		VkDescriptorBufferInfo bufferInfo{};
		//bufferInfo.buffer = api->GetUniformBuffers()[iSet][0];
		bufferInfo.buffer = m_uniformBuffer.buffer;
		bufferInfo.offset = m_alignedSize * iSet;
		bufferInfo.range = sizeof(GraphicAPI::UniformMVP);

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = api->GetDescriptorSets()[m_setID][iSet];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;

		descriptorWrites.push_back(descriptorWrite);

		vkUpdateDescriptorSets(api->GetDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}

void Model::ProcessAlignedSize()
{
	m_alignedSize = GraphicAPI::GetInstance()->Align(sizeof(GraphicAPI::UniformMVP),
	                                                 GraphicAPI::GetInstance()->GetPhysProperties().limits.
	                                                 minUniformBufferOffsetAlignment);
}

void Model::UploadData()
{
	for (auto& targetMesh : m_targetMeshes)
	{
		if (targetMesh.indexBuffer.bufferMemory == VK_NULL_HANDLE)
		{
			targetMesh.indexBuffer = DataBuffers::UploadData(targetMesh.indices, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
		}

		if (targetMesh.vertexBuffer.bufferMemory == VK_NULL_HANDLE)
		{
			targetMesh.vertexBuffer = DataBuffers::UploadData(targetMesh.vertices, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
		}
	}

	m_uniformBuffer = DataBuffers::CreatHostVisibileBuffer(m_alignedSize * GraphicAPI::GetInstance()->GetSwapChain()->ImageCount());
}

void Model::UploadBuffer(size_t currImg)
{
	(void)currImg;
	//void* data;
	//vkMapMemory(GraphicAPI::GetInstance()->GetDevice(), uniformBuffer.bufferMemory, m_alignedSize * currImg, sizeof(GraphicAPI::UniformMVP), 0, &data);
	//memcpy(data, &uniformMVP, sizeof(uniformMVP));
	//vkUnmapMemory(GraphicAPI::GetInstance()->GetDevice(), uniformBuffer.bufferMemory);
}

void Model::CreateModel(std::string_view pathObj)
{
	//Todo add wrapping ?
	//https://learnopengl.com/Model-Loading/Model
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(pathObj.data(), aiProcess_Triangulate | aiProcess_FlipUVs);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		throw std::runtime_error("Error import object!");
	}
	ProcessNode(scene->mRootNode, scene);
	m_matID = static_cast<size_t>(-1);
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		// process vertex positions, normals and texture coordinates
		Vertex vertex;

		Vector3 vecTemp;
		vecTemp.x = mesh->mVertices[i].x;
		vecTemp.y = mesh->mVertices[i].y;
		vecTemp.z = mesh->mVertices[i].z;
		vertex.pos = vecTemp;

		if(mesh->mNormals != NULL)
		{
		vecTemp.x = mesh->mNormals[i].x;
		vecTemp.y = mesh->mNormals[i].y;
		vecTemp.z = mesh->mNormals[i].z;
		vertex.normal = vecTemp;
		}
		else
		{
			vertex.normal = { 0.f,1.f,0.f };
		}

		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
		{
			Vector2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texCoord = vec;
		}
		else
			vertex.texCoord = Vector2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}
	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	// process material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
		// Colors
		aiColor3D color;
		float fValue;
		Vector3 colorAmbient;
		Vector3 colorDiffuse;
		Vector3 colorSpecular;
		Vector3 shininess;
		Vector3 opacity;

		mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
		colorAmbient = LibMath::Vector3(color.r, color.g, color.b);

		mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		colorDiffuse = LibMath::Vector3(color.r, color.g, color.b);

		mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
		colorSpecular = LibMath::Vector3(color.r, color.g, color.b);

		mat->Get(AI_MATKEY_SHININESS, fValue);
		shininess = LibMath::Vector3(fValue);

		mat->Get(AI_MATKEY_OPACITY, fValue);
		opacity = LibMath::Vector3(fValue);
	}
	Mesh myMesh{ vertices, indices };
	return myMesh;
}


void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_targetMeshes.push_back(ProcessMesh(mesh, scene));
		//todo
		//meshes.push_back(ProceesMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

void Model::CreateDefault()
{
	m_targetMeshes.reserve(1);
	m_targetMeshes.push_back(Mesh{});
	m_targetMeshes[0].vertices = {
		{{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, TVector2<float>{0.0f, 0.0f}},
		{{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f},  TVector2<float>{1.0f, 0.0f}},
		{{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f},  TVector2<float>{1.0f, 1.0f}},
		{{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, TVector2<float> {0.0f, 1.0f}},

		{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f},  TVector2<float>{0.0f, 0.0f}},
		{{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f},  TVector2<float>{1.0f, 0.0f}},
		{{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f},  TVector2<float>{1.0f, 1.0f}},
		{{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f},  TVector2<float>{0.0f, 1.0f}}
	};

	m_targetMeshes[0].indices = {
		0, 1, 2, 2, 3, 0,
		4, 5, 6, 6, 7, 4
	};

	m_matID = 0;
}

void Model::CleanupModel()
{
	GraphicAPI* api = GraphicAPI::GetInstance();

	api->MutFreeIDModelDescriptorSets().push(m_setID);
	vkDestroyBuffer(api->GetDevice(), m_uniformBuffer.buffer, nullptr);
	vkFreeMemory(api->GetDevice(), m_uniformBuffer.bufferMemory, nullptr);
	m_targetMeshes.clear();
}