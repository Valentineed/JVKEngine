#pragma once
#include <vector>
#include <vulkan/vulkan_core.h>

#include "Common.h"
#include "Vector/TVector2.h"
class Window;

struct AllocatedImage
{
	VkImage image;
	VkDeviceMemory allocation;
	VkImageView imageView;
};

class SwapChain
{
public:
	SwapChain() = delete;
	
	/**
	* @brief Create an infrastructure that will own the buffers we will render to before we visualize them on the screen.
	* The swap chain is essentially a queue of images that are waiting to be presented to the screen.
	* General purpose of the swap chain is to synchronize the presentation of images with the refresh rate of the screen.
	* 
	* @param User window
	*/
	SwapChain(Window* window, VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, bool aliasing);
	
	/**
	* @brief /*This image view quite literally corresponds to a view in the image.
	*It describes the access to the image and the parts of the image to access.
	*For example it indicates if it should be treated as a 2D texture for depth without any level of mipmapping.
	*
	*@param Logical device that creates the image view.
	*@param Represent if program use aliasing or not
	*/
	void CreateImageViews(VkDevice device, bool aliasing);

	/**
	 *@brief Get Swap Chain image views count
	 * 
	 */
	size_t ImageCount() const;
	
	[[nodiscard]] VkExtent2D GetSwapChainExtent()const;

	VkImageView GetSwapChainImageView(size_t index);
	
	AllocatedImage& GetImageOffScreen(size_t index);
	std::vector<AllocatedImage>& GetOffScreenImages();
	
	VkImageView GetDepthImageView();
	VkImageView GetMSAAImageView();
	
	size_t m_imageViewCount = 0;

	/**
	* @brief Checking for swap chain support: capabilities, surface formats and  presentation modes
	*
	*  @param Current device
	*  @param Surface window
	*/
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) const;


	/**
	* @brief Create an image view from an existing image
	*
	*@param Logical device that creates the image view.
	*@param Opaque handle to an image object
	*@param Available image formats
	*@param Bitmask of VkImageAspectFlagBits
	*/
	VkImageView CreateImageView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags);

	void RecreateSwapChain(Window* window, VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, bool aliasing);

	void DestroyStorageImages();
	VkFormat GetFormat();
	VkSwapchainKHR Get();
	
	/*
	* @brief Search format depthBuffer
	*/
	VkFormat FindDepthFormat();
private:
	/*OffScreen Images Storage*/
	std::vector<AllocatedImage> m_offscreenImages;
	
	/*DepthBuffer Storage*/
	AllocatedImage m_depthImages;

	/*MSAA Image Storage*/
	AllocatedImage m_msaaImages;

	std::vector<VkImage> m_swapChainImages;
	std::vector<VkImageView> m_swapChainImageViews;
	VkFormat m_swapChainImageFormat;
	VkExtent2D m_swapChainExtent;
	VkSwapchainKHR m_swapChain;
	
	/**
	* @brief Create images offscreen
	* 
	*@param Logical device that creates the image view.
	*/
	void InitImgsOffScreen(VkDevice device);
	
	/**
	* @brief Create DepthBuffers
	*/
	void CreateDepthResources();

	/*
	* @brief Search format depthBuffer
	*/
	VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	
	/**
	 *@brief Create MSAA Image
	 */
	void CreateMSAAImageView();

	/**
	* @brief  List and see if the preferred combination is available
	*
	*  @param availableFormats compatible in surface set in QuerySwapChainSupport(VkPhysicalDevice device)
	*/
	VkSurfaceFormatKHR  ChooseSwapChainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

	/**
	* @brief This function sets the displayed image on screen conditions priority in triple buffering
	*
	*  @param availablePresentModes compatible in surface
	*/
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);


	/**
	* @brief The swap extent gives the resolution of the images in the swap chain and almost always corresponds to the resolution of the window we are using
	*
	*  @param SwapChain capabilities
	*  @param User window
	*/
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, Window* window) const;

	/**
	* @brief Find the family indices to add to the structure
	*
	*  @param Current device
	*  @Surface window
	*/
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
	
	void CreateSwapChain(Window* window, VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, bool aliasing);
};
