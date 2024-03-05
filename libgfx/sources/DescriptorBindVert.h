#pragma once
#include <vulkan\vulkan.h>

struct VP
{
	Matrix4 view;
	Matrix4 perspective;
};

struct DescriptorBindVertexLayout
{
	uint32_t binding;
	VkDescriptorType descriptorType;
	uint32_t typeStride;
	uint32_t descriptorArraySize;	//e.g. if we'd normally pass '1' bone then it'd only be 1 in size, but normally it'd be N where N is num of bones.

	static constexpr DescriptorBindVertexLayout MakeDescriptorLayout(uint32_t binding, VkDescriptorType descriptorType)
	{
		return { binding, descriptorType, 1 };
	}

	template<typename type, uint32_t binding>
	static constexpr DescriptorBindVertexLayout MakeDescriptorLayout(VkDescriptorType descriptor_type)
	{
		return { binding, descriptor_type, sizeof(type), 1 };
	}
	template<typename type>
	static constexpr DescriptorBindVertexLayout MakeDescriptorLayout(uint32_t binding, VkDescriptorType descriptorType)
	{
		return { binding, descriptorType, sizeof(type),1 };
	}
	template<typename type>
	static constexpr DescriptorBindVertexLayout MakeDescriptorLayout(type&&, uint32_t binding, VkDescriptorType descriptorType)
	{
		return { binding, descriptorType, sizeof(type), 1 };
	}
};

struct DescriptorBindVertexBuffer
{
	DescriptorBindVertexLayout layout;
	VkBuffer buffer;

	static DescriptorBindVertexBuffer MakeDescriptorBuffer(DescriptorBindVertexLayout layout, VkBuffer buffer)
	{
		return { layout, buffer };
	}
};