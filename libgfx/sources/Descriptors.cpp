#include "Descriptors.h"
#include "RenderingManager.h"
#include "Core/SwapChain.h"

void DescriptorSet::CreateDescriptorPool(std::vector<DescriptorSetLayout> setLayouts)
{
	GraphicAPI* api = GraphicAPI::GetInstance();

	//! IF EVER CRASHES IN UPDATE DESCRIPTOR SETS NEED TO RECREATE POOL AND STORE INDEX OF CURRENTLY RUNNING POOL !

	//std::array<VkDescriptorPoolSize, 2> poolSizes{};
	//poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	//poolSizes[0].descriptorCount = static_cast<uint32_t>(2 * api->GetSwapChainImages().size() * MAX_SETS_IN_POOL);
	//poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	//poolSizes[1].descriptorCount = static_cast<uint32_t>(api->GetSwapChainImages().size() * MAX_SETS_IN_POOL);

	std::vector<VkDescriptorPoolSize> poolSizes{};
	const size_t swapChainImgCount = api->GetSwapChain()->ImageCount();
	for (int i = 0; i < setLayouts.size(); i++)
	{
		for (auto& vertBinding : setLayouts[i].vertexBindings)
			poolSizes.push_back({ vertBinding.descriptorType, static_cast<uint32_t>(swapChainImgCount * MAX_SETS_IN_POOL) });
	}
	for (int i = 0; i < setLayouts.size(); i++)
	{
		for (auto& fragbinding : setLayouts[i].fragmentBindings)
			poolSizes.push_back({ fragbinding.descriptorType, static_cast<uint32_t>(swapChainImgCount * MAX_SETS_IN_POOL) });
		//image array size is assumed one for the time being.
	}

	VkDescriptorPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
	poolInfo.pPoolSizes = poolSizes.data();
	poolInfo.maxSets = static_cast<uint32_t>(swapChainImgCount * MAX_SETS_IN_POOL);

	if (vkCreateDescriptorPool(api->GetDevice(), &poolInfo, nullptr, &api->MutDescriptorPool()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create descriptor pool!");
	}
}
//CreateDescriptorSet() func in light or model needing obj to get called, same for creationof descSetLayout,
//Check for binding creation, (All binding will prolly be 0 if we go for 1 descSet for 1 uniformVariable but setID is different) ex 
//(set = 0, binding = 0) light
//(set = 1, binding = 0) texture
//(set = 2, binding = 0) camera
//(set = 3, binding = 0) model
//Check for pipeline creation, VkPipelineLayoutCreateInfo l.85, prolly descCount = 3, and give it vector<VkDescriptorSetLayout> instead of one
//So we need definiton of a createDescset in every class corresponding to a uniform
//Ex : for (auto& model : models){ model.CreateDescSet()}
//CreateUniformBuffer discard, we keep DataBuffer in class for example and dont forget to call CreatHostVisibileBuffer()
void DescriptorSet::CreateDescriptorSets(std::vector<Model>& models)
{
	GraphicAPI* api = GraphicAPI::GetInstance();

	//--------------------------------------------------------------------Test--------------------------------------------------------------------

	VkDescriptorSetVariableDescriptorCountAllocateInfoEXT variableDescriptorCountAllocInfo = {};

	uint32_t variableDescCounts[] = { static_cast<uint32_t>(textures.size()) };

	variableDescriptorCountAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO_EXT;
	variableDescriptorCountAllocInfo.descriptorSetCount = 1;
	variableDescriptorCountAllocInfo.pDescriptorCounts = variableDescCounts;

	const size_t swapChainImgCount = api->GetSwapChain()->ImageCount();
	//--------------------------------------------------------------------------------------------------------------------------------------------
	//Same nb of layout as descriptorSetCount
	std::vector<VkDescriptorSetLayout> layouts(swapChainImgCount, descriptorSetLayout.setLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = api->GetDescriptorPool();	//If using Pool of pool method, need to dynamically swap descpPool
	allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImgCount);
	allocInfo.pSetLayouts = layouts.data();

	//--------------------------------------------------------------------Test--------------------------------------------------------------------
	//allocInfo.pNext = &variableDescriptorCountAllocInfo;

	//--------------------------------------------------------------------Test--------------------------------------------------------------------

	//Assuming 1 model = 1 material and 1 material = 1 texture
	if (models.size() > 0)
	{
		for (size_t iMod = 0; iMod < models.size() - 1; iMod++)
		{
			if (models[iMod].m_matID == models[models.size() - 1].m_matID)
			{
				return;
			}
		}
	}
	std::vector<VkDescriptorSet>* NewMaterial = __nullptr;

	NewMaterial = &api->MutDescriptorSets().emplace_back();
	NewMaterial->resize(swapChainImgCount);

	if (vkAllocateDescriptorSets(api->GetDevice(), &allocInfo, (*NewMaterial).data()) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate descriptor sets!");
	}

	for (int iSets = 0; iSets < NewMaterial->size(); iSets++)
	{
		std::vector<VkWriteDescriptorSet> descriptorWrites;

		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = api->GetUniformBuffers()[iSets][0];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(GraphicAPI::UniformBufferObject);

		//std::vector<VkDescriptorImageInfo> imageInfo(textures.size());
		//for (size_t j = 0; j < textures.size(); j++)
		//{
		//	imageInfo[j].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		//	imageInfo[j].sampler = models[i].targetSampler.sampler;
		//	imageInfo[j].imageView = textures[models[i].texID].imageView;
		//}

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		//imageInfo.sampler = models[models.size() - 1].targetSampler.sampler;
		//imageInfo.imageView = models[i].targetTexture.imageView;
		if (textures.size() > 1)
		{
			imageInfo.imageView = textures[models[models.size() - 1].m_matID].image.m_imageView;
		}
		else
		{
			//Assume that id 0 is always defaultTexture
			imageInfo.imageView = textures[0].image.m_imageView;
		}

		VkDescriptorBufferInfo lightInfo{};
		lightInfo.buffer = api->GetUniformBuffers()[iSets][1];
		lightInfo.offset = 0;
		lightInfo.range = sizeof(GraphicAPI::LightBufferObject);

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = (*NewMaterial)[iSets];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;

		descriptorWrites.push_back(descriptorWrite);

		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = (*NewMaterial)[iSets];
		descriptorWrite.dstBinding = 1;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pImageInfo = &imageInfo;

		descriptorWrites.push_back(descriptorWrite);

		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = (*NewMaterial)[iSets];
		descriptorWrite.dstBinding = 2;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &lightInfo;

		descriptorWrites.push_back(descriptorWrite);

		vkUpdateDescriptorSets(api->GetDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}
}