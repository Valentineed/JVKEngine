#pragma once
#include <vulkan/vulkan_core.h>

struct SurfaceDescriptor
{
	SurfaceDescriptor(VkInstance ista, VkAllocationCallbacks* alloc, VkSurfaceKHR* surf) :
		instance(ista), allocator(alloc), surface(surf)
	{
	}
	VkInstance instance = nullptr;
	const VkAllocationCallbacks* allocator = nullptr;
	VkSurfaceKHR* surface = nullptr;
};
