#pragma once
#include <vulkan/vulkan.h>

#include <vector>
#include <iostream>
#include <optional>

//Macro to check vulkan return results
//basically if this is triggered everythings broken.
#define CHECK_VK_RESULT(result)					\
{												\
	if(result != VK_SUCCESS)					\
	{											\
		std::cout << "Error : VkResult is \"" << error_string(result) << "\" in " << __FILE__ << " at line" << __LINE__ << std::endl;\
		assert(result == VK_SUCCESS);			\
	}											\
}

struct QueueFamilyIndices
{
	//the std::optional allows the distinction between the case where the value exists and the one where it does not exist
	std::optional<uint32_t> m_graphicsFamily;
	std::optional<uint32_t> m_presentFamily;

	bool IsComplete()
	{
		return m_graphicsFamily.has_value() && m_presentFamily.has_value();
	}
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

std::vector<char> ReadFile(const std::string& filename);