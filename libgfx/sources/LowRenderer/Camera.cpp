#include "Camera.h"

#include "Descriptors.h"
#include "Core/SwapChain.h"
namespace LowRenderer
{
	VkDeviceSize Camera::m_alignedSize = 0;
	
	Camera::Camera()
	{
		UploadData();
	}

	Camera::~Camera()
	{
		Destroy();
	}

	void Camera::ProcessAlignedSize()
	{
		m_alignedSize = GraphicAPI::GetInstance()->Align(sizeof(GraphicAPI::UniformViewPos),
			GraphicAPI::GetInstance()->GetPhysProperties().limits.
			minUniformBufferOffsetAlignment);
	}

	void Camera::Destroy()
	{
		GraphicAPI* api = GraphicAPI::GetInstance();

		vkDestroyBuffer(api->GetDevice(), m_cameraBuffer.buffer, nullptr);
		vkFreeMemory(api->GetDevice(), m_cameraBuffer.bufferMemory, nullptr);
	}

	void Camera::UploadData()
	{
		m_cameraBuffer = DataBuffers::CreatHostVisibileBuffer(m_alignedSize * GraphicAPI::GetInstance()->GetSwapChain()->ImageCount());
	}

	void Camera::CreateDescriptorSet(DescriptorSetLayout& setLayout)
	{
		GraphicAPI* api = GraphicAPI::GetInstance();

		const size_t swapChainImageCount = api->GetSwapChain()->ImageCount();
		std::vector<VkDescriptorSetLayout> layouts(swapChainImageCount, setLayout.setLayout);
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = api->GetDescriptorPool();	//If using Pool of pool method, need to dynamically swap descpPool
		allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImageCount);
		allocInfo.pSetLayouts = layouts.data();

		std::vector<VkDescriptorSet>* NewSet = __nullptr;

		NewSet = &api->MutDescriptorSets().emplace_back();
		m_setID = api->GetDescriptorSets().size() - 1;
		NewSet->resize(swapChainImageCount);

		if (vkAllocateDescriptorSets(api->GetDevice(), &allocInfo, (*NewSet).data()) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate descriptor sets!");
		}
	}

	void Camera::UpdateDescriptorSet()
	{
		GraphicAPI* api = GraphicAPI::GetInstance();

		for (int iSet = 0; iSet < api->GetDescriptorSets()[m_setID].size(); iSet++)
		{
			std::vector<VkWriteDescriptorSet> descriptorWrites;

			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.buffer = m_cameraBuffer.buffer;
			bufferInfo.offset = m_alignedSize * iSet;
			bufferInfo.range = sizeof(GraphicAPI::UniformViewPos);

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
}