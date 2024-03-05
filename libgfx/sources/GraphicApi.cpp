#include "GraphicAPI.h"
#include "Renderpass.h"
#include "Pipeline.h"
#include "Descriptors.h"
#include "Window/Window.h"
#include "SurfaceDescriptor.h"

#include <algorithm>
#include <set>
#include <stdexcept>
#include <GLFW/glfw3.h>


#include "Defines.h"
#include "Core/GFXAPI.h"
#include "Core/SwapChain.h"


GraphicAPI* GraphicAPI::GetInstance()
{
	static GraphicAPI instance;

	return &instance;
}

QueueFamilyIndices GraphicAPI::FindQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;// Code to find the family indices to add to the structure

		//vkGetPhysicalDeviceQueueFamilyProperties = get the list of available queues
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	//check if one queue support VK_QUEUE_GRAPHICS_BIT
	int i = 0;
	for (const auto& queueFamily : queueFamilies)
	{
		if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.m_graphicsFamily = i;
		}

		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface, &presentSupport);

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

//if multithread queue all VkQueueSubmit (save VkSubmitInfo) before each frame
void GraphicAPI::EndCommandSaveSubmitInfo(VkCommandBuffer commandBuffer, std::pair<VkBuffer, VkDeviceMemory> deletionQueue)
{
	vkEndCommandBuffer(commandBuffer);

	{
		std::lock_guard<std::mutex> lock{ m_mutexSubmit };
		m_submitCmdBuff.push_back(commandBuffer);
		m_submitDeletionQueue.push_back(deletionQueue);
	}
}

void GraphicAPI::EndSingleTimeCommands(VkCommandBuffer commandBuffer)
{
	vkEndCommandBuffer(commandBuffer);

	std::lock_guard<std::mutex> lock{ m_mutexSubmit };
	{
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(m_graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(m_graphicsQueue);

		vkFreeCommandBuffers(m_device, m_commandPool, 1, &commandBuffer);
	}
}

VkCommandBuffer GraphicAPI::BeginSingleTimeCommands()
{
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = m_commandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer commandBuffer;
	vkAllocateCommandBuffers(m_device, &allocInfo, &commandBuffer);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	vkBeginCommandBuffer(commandBuffer, &beginInfo);

	return commandBuffer;
}

void GraphicAPI::CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
	VkCommandBuffer commandBuffer = BeginSingleTimeCommands();

	//need to find a away to preserve this struct after exiting in a sane way.
	//if everything is used this way... vkCmdCopyBuffer will asyncly try to access our struct... which is now garbaged.
	VkBufferCopy copyRegion{};
	copyRegion.size = size;
	vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

	EndSingleTimeCommands(commandBuffer);
}

void GraphicAPI::CreateSyncObjects()
{
	m_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	m_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	m_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
	m_imagesInFlight.resize(m_swapChainClass->ImageCount(), VK_NULL_HANDLE);

	VkSemaphoreCreateInfo semaphoreInfo{};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo{};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		if (vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(m_device, &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(m_device, &fenceInfo, nullptr, &m_inFlightFences[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create synchronization objects for a frame!");
		}
	}
}

GraphicAPI::~GraphicAPI()
{
	delete m_swapChainClass;
}

VkInstance& GraphicAPI::MutVkInstance()
{
	return m_instanceVK;
}

VkPhysicalDevice& GraphicAPI::MutPhysicalDevice()
{
	return m_physicalDevice;
}

VkDevice& GraphicAPI::MutDevice()
{
	return m_device;
}

SwapChain* GraphicAPI::GetSwapChain()
{
	return m_swapChainClass;
}

uint32_t GraphicAPI::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
		{
			return i;
		}
	}
	throw std::runtime_error("failed to find suitable memory type!");
}

void GraphicAPI::RecreateSwapChain(Window* window)
{
	m_swapChainClass->RecreateSwapChain(window, m_device, m_physicalDevice, m_surface, m_bAliasing);
}

void GraphicAPI::DestroyStorageSwapChain()
{
	m_swapChainClass->DestroyStorageImages();
}

bool GraphicAPI::AliasingIsActive()
{
	return m_bAliasing;
}

VkSampleCountFlagBits GraphicAPI::GetMSAASamples()
{
	return m_msaaSamples;
}

bool GraphicAPI::CheckValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for (const char* layerName : g_validationLayers)
	{
		bool bLayerFound = false;

		for (const auto& layerProperties : availableLayers)
		{
			if (strcmp(layerName, layerProperties.layerName) == 0)
			{
				bLayerFound = true;
				break;
			}
		}

		if (!bLayerFound)
		{
			return false;
		}
	}

	return true;
}

std::vector<const char*> GraphicAPI::GetRequiredExtensions(Window* window)
{
	uint32_t extensionCount = 0;
	const char** windowExtensions = window->GetRequiredExtensions(&extensionCount);
	if (windowExtensions == nullptr)
	{
		throw std::runtime_error("Error to search a glfwExtensions");
	}

	std::vector<const char*> extensions(windowExtensions, windowExtensions + extensionCount);

	if (g_enableValidationLayers)
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

bool GraphicAPI::IsDeviceSuitable(VkPhysicalDevice device)
{
	QueueFamilyIndices indices = FindQueueFamilies(device);

	const bool bExtensionsSupported = CheckDeviceExtensionSupport(device);

	bool bSwapChainAdequate = false;
	if (bExtensionsSupported)
	{
		const SwapChainSupportDetails swapChainSupport = m_swapChainClass->QuerySwapChainSupport(device, m_surface);
		bSwapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	return indices.IsComplete() && bExtensionsSupported && bSwapChainAdequate;
}

bool GraphicAPI::CheckDeviceExtensionSupport(VkPhysicalDevice device)
{
	uint32_t extensionCount;
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

	std::vector<VkExtensionProperties> availableExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

	std::set<std::string> requiredExtensions(g_deviceExtensions.begin(), g_deviceExtensions.end());

	for (const auto& extension : availableExtensions)
	{
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

VkSampleCountFlagBits GraphicAPI::GetMaxUsableSampleCount()
{
	VkPhysicalDeviceProperties physicalDeviceProperties;
	vkGetPhysicalDeviceProperties(GraphicAPI::GetInstance()->GetPhysicalDevice(), &physicalDeviceProperties);

	VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;
	if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
	if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
	if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
	if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
	if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
	if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

	return VK_SAMPLE_COUNT_1_BIT;
}



void GraphicAPI::CreateAPIInstance(Window* window)
{
	if (window == nullptr)
	{
		throw std::runtime_error("Window in Graphic API is nullptr!");
	}

	/*Set Version*/
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Libgfx";
	appInfo.pEngineName = "JVK Engine";
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.apiVersion = VK_API_VERSION_1_0;

	//Inform the driver of the extensions and validate layers that we will use
	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	auto extensions = GetRequiredExtensions(window);
	createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
	createInfo.ppEnabledExtensionNames = extensions.data();

	VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
	if (g_enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(g_validationLayers.size());
		createInfo.ppEnabledLayerNames = g_validationLayers.data();

		Debug::PopulateDebugMessengerCreateInfo(debugCreateInfo);
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
	}
	else
	{
		createInfo.enabledLayerCount = 0;

		createInfo.pNext = nullptr;
	}

	if (GFXAPI::CreateVulkanInstance(&createInfo, nullptr, &m_instanceVK) == NOT_SUCCESS)
	{
		throw std::runtime_error("failed to create m_instanceVK!");
	}
}

void GraphicAPI::Init(Window* window, bool aliasing)
{
	m_bAliasing = aliasing;
	CreateAPIInstance(window);
	SetupVulkanDebugCallback();
	CreateWindowSurface(window);
	PickPhysicalDevice();
	GFXAPI::GetVulkanPhysicalProperties(m_physicalDevice, &m_properties);
	CreateLogicalDevice();

	m_swapChainClass = new SwapChain(window, m_device, m_physicalDevice, m_surface, aliasing);

}

void GraphicAPI::CreateWindowSurface(Window* window)
{
	if (window == nullptr)
	{
		throw std::runtime_error("Window in Graphic API is nullptr!");
	}
	SurfaceDescriptor sDescriptor(m_instanceVK, nullptr, &m_surface);

	if ((VkResult)window->CreateSurface(&sDescriptor) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create m_window m_surface!");
	}
}

void GraphicAPI::PickPhysicalDevice()
{
	//Search for GPU
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(m_instanceVK, &deviceCount, nullptr);
	if (deviceCount == 0)
	{
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	//Allocates an array containing all references to VkPhysicalDevice.
	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(m_instanceVK, &deviceCount, devices.data());

	//Search for ideal GPU
	for (const auto& device : devices)
	{
		if (IsDeviceSuitable(device))
		{
			m_physicalDevice = device;
			if(m_bAliasing)
			{
				m_msaaSamples = GetMaxUsableSampleCount();
			}
			break;
		}
	}

	if (m_physicalDevice == VK_NULL_HANDLE)
	{
		throw std::runtime_error("failed to find a suitable GPU!");
	}
}

void GraphicAPI::CreateLogicalDevice()
{
	QueueFamilyIndices indices = FindQueueFamilies(m_physicalDevice);

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	std::set<uint32_t> uniqueQueueFamilies = { indices.m_graphicsFamily.value(), indices.m_presentFamily.value() };

	//Assigns priority levels to queues
	float queuePriority = 1.0f;
	for (uint32_t queueFamily : uniqueQueueFamilies)
	{
		VkDeviceQueueCreateInfo queueCreateInfo{};
		queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queueCreateInfo.queueFamilyIndex = queueFamily;
		queueCreateInfo.queueCount = 1;
		queueCreateInfo.pQueuePriorities = &queuePriority;
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkPhysicalDeviceFeatures deviceFeatures{};

	deviceFeatures.samplerAnisotropy = true;

	//Structure specifying parameters of a newly created devices
	VkDeviceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	createInfo.pQueueCreateInfos = queueCreateInfos.data();
	createInfo.pEnabledFeatures = &deviceFeatures;
	createInfo.enabledExtensionCount = static_cast<uint32_t>(g_deviceExtensions.size());
	createInfo.ppEnabledExtensionNames = g_deviceExtensions.data();

	if (g_enableValidationLayers)
	{
		createInfo.enabledLayerCount = static_cast<uint32_t>(g_validationLayers.size());
		createInfo.ppEnabledLayerNames = g_validationLayers.data();
	}
	else
	{
		createInfo.enabledLayerCount = 0;
	}

	//Create a logical device
	if (GFXAPI::CreateVulkanDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != SUCCESS)
	{
		throw std::runtime_error("failed to create logical device!");
	}

	// Get a queue handle from a device
	GFXAPI::GetVulkanDeviceQueue(m_device, indices.m_graphicsFamily.value(), 0, &m_graphicsQueue);
	GFXAPI::GetVulkanDeviceQueue(m_device, indices.m_presentFamily.value(), 0, &m_presentQueue);
}

void GraphicAPI::CreateFramebuffers(VkRenderPass* rPass)
{
	// Create Info for Offscreen Framebuffers
	VkFramebufferCreateInfo fb_offscreen_info = {};
	fb_offscreen_info.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	fb_offscreen_info.pNext = nullptr;

	fb_offscreen_info.renderPass = *rPass;
	fb_offscreen_info.width = m_swapChainClass->GetSwapChainExtent().width;
	fb_offscreen_info.height = m_swapChainClass->GetSwapChainExtent().height;
	fb_offscreen_info.layers = 1;

	// Grab how many images we have in swapchain
	const size_t swapchain_imagecount = m_swapChainClass->ImageCount();
	m_swapChainFramebuffers.clear();
	m_swapChainFramebuffers = std::vector<VkFramebuffer>(swapchain_imagecount);

	// Create Framebuffers for each of the swapchain image views
	const size_t imageViewCount = m_swapChainClass->m_imageViewCount;
	for (int i = 0; i < swapchain_imagecount; i++)
	{
		if(imageViewCount == 3)
		{
			// Attach offscreen and depth image view to Framebuffer
			std::array<VkImageView, 3> attachmentsMSAA = {
				m_swapChainClass->GetImageOffScreen(i).imageView,
				m_swapChainClass->GetDepthImageView(),
				m_swapChainClass->GetMSAAImageView(),
			};
			fb_offscreen_info.pAttachments = attachmentsMSAA.data();
			fb_offscreen_info.attachmentCount = static_cast<uint32_t>(attachmentsMSAA.size());
		}
		else
		{
			std::array<VkImageView, 2> attachments = {
				m_swapChainClass->GetImageOffScreen(i).imageView,
				m_swapChainClass->GetDepthImageView(),
			};
			fb_offscreen_info.pAttachments = attachments.data();
			fb_offscreen_info.attachmentCount = static_cast<uint32_t>(attachments.size());
		}
		

		if (vkCreateFramebuffer(m_device, &fb_offscreen_info, nullptr, &m_swapChainFramebuffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create framebuffer!");
		}
	}
}

void GraphicAPI::CreateImGuiFramebuffers(VkRenderPass* rPass)
{
	const size_t swapChainImagCount = m_swapChainClass->ImageCount();
	m_ImGuiFramebuffers.resize(swapChainImagCount);

	VkFramebufferCreateInfo framebufferInfo{};
	framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	framebufferInfo.renderPass = *rPass;
	framebufferInfo.width = m_swapChainClass->GetSwapChainExtent().width;
	framebufferInfo.height = m_swapChainClass->GetSwapChainExtent().height;
	framebufferInfo.layers = 1;
	framebufferInfo.attachmentCount = 1;
	
	for (size_t i = 0; i < swapChainImagCount; i++)
	{
		VkImageView attachments = {
			m_swapChainClass->GetSwapChainImageView(i),
		};

		framebufferInfo.pAttachments = &attachments;

		if (vkCreateFramebuffer(m_device, &framebufferInfo, nullptr, &m_ImGuiFramebuffers[i]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create framebuffer!");
		}
	}
}

void GraphicAPI::CreateCommandPool()
{
	//Command pools manages the memory to be used by buffers
	QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(m_physicalDevice);

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.queueFamilyIndex = queueFamilyIndices.m_graphicsFamily.value();

	if (vkCreateCommandPool(m_device, &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create command pool!");
	}
}

void GraphicAPI::CreateImGuiCommandPool()
{
	//Command pools manages the memory to be used by buffers
	QueueFamilyIndices queueFamilyIndices = FindQueueFamilies(m_physicalDevice);

	VkCommandPoolCreateInfo poolInfo{};
	poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	poolInfo.queueFamilyIndex = queueFamilyIndices.m_graphicsFamily.value();

	if (vkCreateCommandPool(m_device, &poolInfo, nullptr, &m_ImGuiCommandPool) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create command pool!");
	}
}

void GraphicAPI::SetupVulkanDebugCallback()
{
	m_debug.SetupDebugMessenger();
}
