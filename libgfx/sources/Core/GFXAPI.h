#pragma once
#include <vulkan/vulkan_core.h>

namespace GFXAPI
{

	/**
	* @brief Create a new Vulkan instance
	*
	* @param pCreateInfo is a pointer to a VkInstanceCreateInfo structure controlling creation of the instance.
	* @param pAllocator controls host memory allocation
	* @param pInstance points a VkInstance handle in which the resulting instance is returned.
	*
	* @return return if SUCCESS
	*/
	int CreateVulkanInstance(const VkInstanceCreateInfo* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkInstance* pInstance);
	/**
	* @brief  Create a new device instance
	*
	* @param physicalDevice user physical device
	* @param pCreateInfo is a pointer to a VkDeviceCreateInfo structure containing information about how to create the device.
	* @param ppAllocator controls host memory allocation
	* @param pDevice is a pointer to a handle in which the created VkDevice is returned.
	*
	* @return return if SUCCESS
	*/
	int CreateVulkanDevice(VkPhysicalDevice physicalDevice,
		const VkDeviceCreateInfo* pCreateInfo,
		const VkAllocationCallbacks* pAllocator,
		VkDevice* pDevice);
	
	/**
	* @brief properties of a physical device
	*
	* @param physicalDevice is the handle to the physical device whose properties will be queried.
	* @param pProperties is a pointer to a VkPhysicalDeviceProperties structure in which properties are returned.
	*/
	void GetVulkanPhysicalProperties(VkPhysicalDevice physicalDevice,
		VkPhysicalDeviceProperties* pProperties);
	/**
	* @brief Get a queue handle from a device
	*
	* @param device is the logical device that owns the queue.
	* @param queueFamilyIndex is the index of the queue family to which the queue belongs.
	* @param queueIndex is the index within this queue family of the queue to retrieve.
	* @param pQueue is a pointer to a VkQueue object that will be filled with the handle for the requested queue.
	*/
	void GetVulkanDeviceQueue(
		VkDevice                                    device,
		uint32_t                                    queueFamilyIndex,
		uint32_t                                    queueIndex,
		VkQueue* pQueue);
	
	/**
	* @brief Query surface capabilities
	*
	* @param physicalDevice is the physical device that will be associated with the swapchain to be created, as described for vkCreateSwapchainKHR.
	* @param surface is the surface that will be associated with the swapchain.
	* @param pSurfaceCapabilities is a pointer to a VkSurfaceCapabilitiesKHR structure in which the capabilities are returned.
	*
	* @return return if SUCCESS
	*/
	int GetVulkanPhysicalDeviceSurfaceCapabilitiesKHR(
		VkPhysicalDevice                            physicalDevice,
		VkSurfaceKHR                                surface,
		VkSurfaceCapabilitiesKHR* pSurfaceCapabilities);
	/**
	* @brief Query color formats supported by surface
	*
	* @param physicalDevice is the physical device that will be associated with the swapchain to be created, as described for vkCreateSwapchainKHR.
	* @param surface is the surface that will be associated with the swapchain.
	* @param pSurfaceFormatCount is a pointer to an integer related to the number of format pairs available or queried, as described below.
	* @param pSurfaceFormats is either NULL or a pointer to an array of VkSurfaceFormatKHR structures.
	* 
	* @return return if SUCCESS
	*/
	int GetVulkanPhysicalDeviceSurfaceFormatsKHR(
		VkPhysicalDevice                            physicalDevice,
		VkSurfaceKHR                                surface,
		uint32_t* pSurfaceFormatCount,
		VkSurfaceFormatKHR* pSurfaceFormats);
	
	/**
	* @brief Query supported presentation modes
	*
	* @param physicalDevice is the physical device that will be associated with the swapchain to be created, as described for vkCreateSwapchainKHR.
	* @param surface is the surface that will be associated with the swapchain.
	* @param pPresentModeCount is a pointer to an integer related to the number of presentation modes available or queried, as described below.
	* @param pPresentModes is either NULL or a pointer to an array of VkPresentModeKHR values, indicating the supported presentation modes.
	* 
	* @return return if SUCCESS
	*/
	int GetVulkanPhysicalDeviceSurfacePresentModesKHR(
		VkPhysicalDevice                            physicalDevice,
		VkSurfaceKHR                                surface,
		uint32_t* pPresentModeCount,
		VkPresentModeKHR* pPresentModes);
}