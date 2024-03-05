#pragma once
#include <vulkan\vulkan.h>

#include "GraphicAPI.h"
#include "Common.h"

struct DataBuffers
{
	VkBuffer buffer = VK_NULL_HANDLE;
	VkDeviceMemory bufferMemory = VK_NULL_HANDLE;

	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties/*, VkBuffer& buffer, VkDeviceMemory& bufferMemory*/)
	{
		GraphicAPI* api = GraphicAPI::GetInstance();

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(api->GetDevice(), &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create buffer!");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(api->GetDevice(), buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = api->FindMemoryType(memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(api->GetDevice(), &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate buffer memory!");
		}

		vkBindBufferMemory(api->GetDevice(), buffer, bufferMemory, 0);
		//std::cout << "buffer : " << buffer << "  memory : " << bufferMemory << "\n";
	}

	static void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& bufferVK, VkDeviceMemory& bufferMemoryVK)
	{
		GraphicAPI* api = GraphicAPI::GetInstance();

		VkBufferCreateInfo bufferInfo{};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = size;
		bufferInfo.usage = usage;
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(api->GetDevice(), &bufferInfo, nullptr, &bufferVK) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create buffer!");
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(api->GetDevice(), bufferVK, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = api->FindMemoryType(memRequirements.memoryTypeBits, properties);

		if (vkAllocateMemory(api->GetDevice(), &allocInfo, nullptr, &bufferMemoryVK) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate buffer memory!");
		}

		vkBindBufferMemory(api->GetDevice(), bufferVK, bufferMemoryVK, 0);
	}

	static void CreateUniformBuffers();

	template<typename type>
	static DataBuffers UploadData(const std::vector<type>& bufferData, VkBufferUsageFlags flags)
	{
		GraphicAPI* api = GraphicAPI::GetInstance();

		DataBuffers dataBuffer;
		DataBuffers stagingBuffer;
		VkDeviceSize bufferSize = sizeof(type) * bufferData.size();

		stagingBuffer.CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		void* data;
		vkMapMemory(api->GetDevice(), stagingBuffer.bufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, bufferData.data(), (size_t)bufferSize);
		vkUnmapMemory(api->MutDevice(), stagingBuffer.bufferMemory);

		dataBuffer.CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | flags, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		api->CopyBuffer(stagingBuffer.buffer, dataBuffer.buffer, bufferSize);

		vkDestroyBuffer(api->GetDevice(), stagingBuffer.buffer, nullptr);
		vkFreeMemory(api->GetDevice(), stagingBuffer.bufferMemory, nullptr);

		return dataBuffer;
	}

	static DataBuffers CreatHostVisibileBuffer(VkDeviceSize bufferSize)
	{
		DataBuffers dataBuffer;

		dataBuffer.CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		return dataBuffer;
	}
};