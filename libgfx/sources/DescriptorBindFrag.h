#pragma once
#include <vulkan\vulkan.h>

struct DescriptorBindFragmentLayout
{
	uint32_t binding;
	VkDescriptorType descriptorType;
	static DescriptorBindFragmentLayout MakeDescriptorLayout(uint32_t binding, VkDescriptorType descriptorType)
	{
		return { binding, descriptorType };
	}
};
struct DescriptorBindFragmentBuffer
{
	DescriptorBindFragmentLayout layout;
	VkSampler sampler;
	VkImageView imageView;

	static DescriptorBindFragmentBuffer MakeDescriptorBuffer(DescriptorBindFragmentLayout layout, VkSampler sampler, VkImageView imageView)
	{
		return { layout, sampler, imageView };
	}
};