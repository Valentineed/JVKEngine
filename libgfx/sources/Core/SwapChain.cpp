#include "SwapChain.h"


#include <algorithm>
#include <stdexcept>

#include "Common.h"
#include "Defines.h"
#include "GFXAPI.h"
#include "GraphicAPI.h"
#include "Window/Window.h"

class GraphicAPI;

SwapChain::SwapChain(Window* window, VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, bool aliasing)
{
	CreateSwapChain(window, device, physicalDevice, surface, aliasing);
}

void CreateImage(uint32_t width, uint32_t height, VkSampleCountFlagBits numSample, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
{
	GraphicAPI* api = GraphicAPI::GetInstance();
	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = VK_IMAGE_TYPE_2D;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.samples = numSample;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateImage(api->GetDevice(), &imageInfo, nullptr, &image) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create image!");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(api->GetDevice(), image, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = api->FindMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(api->GetDevice(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate image memory!");
	}

	vkBindImageMemory(api->GetDevice(), image, imageMemory, 0);
}

void SwapChain::CreateImageViews(VkDevice device, bool aliasing)
{
	m_swapChainImageViews.resize(m_swapChainImages.size());

	for (uint32_t i = 0; i < m_swapChainImages.size(); i++)
	{
		m_swapChainImageViews[i] = CreateImageView(device, m_swapChainImages[i], m_swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
	}

	InitImgsOffScreen(device);
	CreateDepthResources();
	m_imageViewCount = 2;
	if(aliasing)
	{
		CreateMSAAImageView();
		m_imageViewCount = 3;
	}
}

size_t SwapChain::ImageCount()const
{
	return m_swapChainImageViews.size();
}

AllocatedImage& SwapChain::GetImageOffScreen(size_t index)
{
	return m_offscreenImages[index];
}

std::vector<AllocatedImage>& SwapChain::GetOffScreenImages()
{
	return m_offscreenImages;
}

VkExtent2D SwapChain::GetSwapChainExtent()const
{
	return m_swapChainExtent;
}

VkImageView SwapChain::GetSwapChainImageView(size_t index)
{
	return m_swapChainImageViews[index];
}

VkImageView SwapChain::GetDepthImageView()
{
	return m_depthImages.imageView;
}

VkImageView SwapChain::GetMSAAImageView()
{
	return m_msaaImages.imageView;
}

VkFormat SwapChain::GetFormat()
{
	return m_swapChainImageFormat;
}

VkSwapchainKHR SwapChain::Get()
{
	return m_swapChain;
}

void SwapChain::InitImgsOffScreen(VkDevice device)
{
	// Grab how many images we have in swapchain
	const size_t swapchain_imagecount = m_swapChainImageViews.size();

	// Create Images/Views for Offscreen Rendering
	m_offscreenImages = std::vector<AllocatedImage>(swapchain_imagecount);

	VkExtent3D imageExtent =
	{
		m_swapChainExtent.width,
		m_swapChainExtent.height,
		1
	};

	for (int i = 0; i < swapchain_imagecount; i++)
	{
		CreateImage(imageExtent.width, imageExtent.height, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_B8G8R8A8_SRGB,
			VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_offscreenImages[i].image, m_offscreenImages[i].allocation);

		//build a image-view for the image to use for rendering
		VkImageViewCreateInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		info.pNext = nullptr;

		info.viewType = VK_IMAGE_VIEW_TYPE_2D;
		info.image = m_offscreenImages[i].image;
		info.format = VK_FORMAT_B8G8R8A8_SRGB;
		info.subresourceRange.baseMipLevel = 0;
		info.subresourceRange.levelCount = 1;
		info.subresourceRange.baseArrayLayer = 0;
		info.subresourceRange.layerCount = 1;
		info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;

		vkCreateImageView(device, &info, nullptr, &m_offscreenImages[i].imageView);

	}
}

void SwapChain::CreateDepthResources()
{
	GraphicAPI* api = GraphicAPI::GetInstance();
	VkFormat depthFormat = FindDepthFormat();

	CreateImage(m_swapChainExtent.width, m_swapChainExtent.height, api->GetMSAASamples(), depthFormat,
		VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_depthImages.image, m_depthImages.allocation);
	m_depthImages.imageView = CreateImageView(api->GetDevice(),m_depthImages.image, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT);
}

VkFormat SwapChain::FindDepthFormat()
{
	return FindSupportedFormat(
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);
}

VkFormat SwapChain::FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
	GraphicAPI* api = GraphicAPI::GetInstance();
	for (VkFormat format : candidates)
	{
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(api->GetPhysicalDevice(), format, &props);

		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
		{
			return format;
		}
		else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
		{
			return format;
		}
	}

	throw std::runtime_error("failed to find supported format!");
}

void SwapChain::CreateMSAAImageView()
{
	GraphicAPI* api = GraphicAPI::GetInstance();
	CreateImage(m_swapChainExtent.width, m_swapChainExtent.height, api->GetMSAASamples(), m_swapChainImageFormat,
		VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_msaaImages.image, m_msaaImages.allocation);
	m_msaaImages.imageView = CreateImageView(api->GetDevice(), m_msaaImages.image, m_swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
}

VkImageView SwapChain::CreateImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags)
{
	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = aspectFlags;		// Used to assume VK_IMAGE_ASPECT_COLOR_BIT;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VkImageView imageView;
	if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create texture image view!");
	}

	return imageView;
}

void SwapChain::RecreateSwapChain(Window* window, VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, bool aliasing)
{
	CreateSwapChain(window, device, physicalDevice, surface, aliasing);
}

void SwapChain::DestroyStorageImages()
{
	GraphicAPI* api = GraphicAPI::GetInstance();
	if(api->AliasingIsActive())
	{
		vkDestroyImageView(api->GetDevice(), m_msaaImages.imageView, nullptr);
		vkDestroyImage(api->GetDevice(), m_msaaImages.image, nullptr);
		vkFreeMemory(api->GetDevice(), m_msaaImages.allocation, nullptr);
	}
	
	for (size_t i = 0; i < m_swapChainImageViews.size(); i++)
	{
		vkDestroyImageView(api->GetDevice(), m_swapChainImageViews[i], nullptr);
	}

	vkDestroyImageView(api->GetDevice(), m_depthImages.imageView, nullptr);
	vkDestroyImage(api->GetDevice(), m_depthImages.image, nullptr);
	vkFreeMemory(api->GetDevice(), m_depthImages.allocation, nullptr);
	
	for (size_t i = 0; i < m_offscreenImages.size(); i++)
	{
		vkDestroyImageView(api->GetDevice(), m_offscreenImages[i].imageView, nullptr);
		vkDestroyImage(api->GetDevice(), m_offscreenImages[i].image, nullptr);
		vkFreeMemory(api->GetDevice(), m_offscreenImages[i].allocation, nullptr);
	}

	vkDestroySwapchainKHR(api->GetDevice(), m_swapChain, nullptr);
}

/*---SwapChain Creation functions---*/
SwapChainSupportDetails SwapChain::QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) const
{
	SwapChainSupportDetails details;

	//Structure describing capabilities of a surface
	if (GFXAPI::GetVulkanPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities) == NOT_SUCCESS)
	{
		throw std::runtime_error("failed GetVulkanPhysicalDeviceSurfaceCapabilitiesKHR!");
	}

	//Recover supported texture formats
	uint32_t formatCount;
	GFXAPI::GetVulkanPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
	if (formatCount != 0)
	{
		details.formats.resize(formatCount);
		GFXAPI::GetVulkanPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
	}

	//Recover the supported presentation modes follows the same principle of surfaces
	uint32_t presentModeCount;
	GFXAPI::GetVulkanPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
	if (presentModeCount != 0)
	{
		details.presentModes.resize(presentModeCount);
		GFXAPI::GetVulkanPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
	}

	return details;
}

VkSurfaceFormatKHR SwapChain::ChooseSwapChainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
	//VK_FORMAT_B8G8R8A8_SRGB = RGBA integers 8bits
	//colorSpace allows you to verify that sRGB is supported using the VK_COLOR_SPACE_SRGB_NONLINEAR_KHR bit field
	for (const auto& availableFormat : availableFormats)
	{
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
		{
			return availableFormat;
		}
	}
	return availableFormats[0];//pick the first format
}

VkPresentModeKHR SwapChain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
	for (const auto& availablePresentMode : availablePresentModes)
	{
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			return availablePresentMode;
		}
	}
	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, Window* window) const
{
	if (capabilities.currentExtent.width != UINT32_MAX)
	{
		return capabilities.currentExtent;
	}
	else
	{
		int width, height;
		window->GetFramebufferSize(&width, &height);

		VkExtent2D actualExtent = {
			static_cast<uint32_t>(width),
			static_cast<uint32_t>(height)
		};

		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));

		return actualExtent;
	}
}

QueueFamilyIndices SwapChain::FindQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
	QueueFamilyIndices indices;// Code to find the family indices to add to the structure

	//vkGetPhysicalDeviceQueueFamilyProperties = get the list of available queues
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

	//check if one queue support VK_QUEUE_GRAPHICS_BIT
	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.m_graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);

		if (presentSupport)
		{
			indices.m_presentFamily = i;
		}

		if (indices.IsComplete())
		{
			break;
		}

		i++;
	}

	return indices;
}

void SwapChain::CreateSwapChain(Window* window, VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, bool aliasing)
{
	const SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport(physicalDevice, surface);

	const VkSurfaceFormatKHR surfaceFormat = ChooseSwapChainSurfaceFormat(swapChainSupport.formats);
	const VkPresentModeKHR presentMode = ChooseSwapPresentMode(swapChainSupport.presentModes);

	const VkExtent2D extent = ChooseSwapExtent(swapChainSupport.capabilities, window);

	//https://vulkan-tutorial.com/fr/Dessiner_un_triangle/Presentation/Swap_chain
	/*The minimum poses a problem. It is possible that the driver makes
	our program wait because it has not finished certain operations, which we do not want.
	It is recommended to use at least one image more than this minimum*/
	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	//The value 0 means that there is no maximum other than memory.
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
	{
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	createInfo.surface = surface;
	createInfo.minImageCount = imageCount;
	createInfo.imageFormat = surfaceFormat.format;
	createInfo.imageColorSpace = surfaceFormat.colorSpace;
	createInfo.imageExtent = extent;
	createInfo.imageArrayLayers = 1;
	createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

	QueueFamilyIndices indices = FindQueueFamilies(physicalDevice, surface);
	uint32_t queueFamilyIndices[] = { indices.m_graphicsFamily.value(), indices.m_presentFamily.value() };

	if (indices.m_graphicsFamily != indices.m_presentFamily)
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = 2;
		createInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else
	{
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
	/*The compositeAlpha field indicates whether the alpha channel should be used to blend colors
	*with those of other windows. To ignore this use VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR*/
	createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	createInfo.presentMode = presentMode;
	createInfo.clipped = VK_TRUE;

	createInfo.oldSwapchain = VK_NULL_HANDLE;

	if (vkCreateSwapchainKHR(device, &createInfo, nullptr, &m_swapChain) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create swap chain!");
	}

	vkGetSwapchainImagesKHR(device, m_swapChain, &imageCount, nullptr);
	m_swapChainImages.resize(imageCount);
	vkGetSwapchainImagesKHR(device, m_swapChain, &imageCount, m_swapChainImages.data());

	m_swapChainImageFormat = surfaceFormat.format;
	m_swapChainExtent = extent;

	CreateImageViews(device, aliasing);
}

/*----------------------------------*/