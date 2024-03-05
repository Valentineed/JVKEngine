#include "..\..\libgfx\sources\Core\GFXAPI.h"

#include "Defines.h"


int GFXAPI::CreateVulkanInstance(const VkInstanceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkInstance* pInstance)
{
	if (vkCreateInstance(pCreateInfo, pAllocator, pInstance) != VK_SUCCESS)
	{
		return NOT_SUCCESS;
	}
	return SUCCESS;
}

int GFXAPI::CreateVulkanDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice)
{
	if (vkCreateDevice(physicalDevice, pCreateInfo, pAllocator, pDevice) != VK_SUCCESS)
	{
		return NOT_SUCCESS;
	}
	return SUCCESS;
}

void GFXAPI::GetVulkanPhysicalProperties(VkPhysicalDevice physicalDevice, VkPhysicalDeviceProperties* pProperties)
{
	vkGetPhysicalDeviceProperties(physicalDevice, pProperties);
}

void GFXAPI::GetVulkanDeviceQueue(VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex, VkQueue* pQueue)
{
	vkGetDeviceQueue(device, queueFamilyIndex, queueIndex, pQueue);
}

int GFXAPI::GetVulkanPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities)
{
	if (vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, pSurfaceCapabilities) != VK_SUCCESS)
	{
		return NOT_SUCCESS;
	}
	return SUCCESS;
}

int GFXAPI::GetVulkanPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount, VkSurfaceFormatKHR* pSurfaceFormats)
{
	if(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats) != VK_SUCCESS)
	{
		return NOT_SUCCESS;
	}
	return SUCCESS;
}

int GFXAPI::GetVulkanPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, uint32_t* pPresentModeCount, VkPresentModeKHR* pPresentModes)
{
	if(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, pPresentModeCount, pPresentModes) != VK_SUCCESS)
	{
		return NOT_SUCCESS;
	}
	return SUCCESS;
}
