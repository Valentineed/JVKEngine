#pragma once

#include <mutex>

#include "Matrix/Matrix.h"
#include "Common.h"
#include "Debug/Debug.h"

#include <vector>
#include <optional>
#include <stack>

#include "libgfx_Export.h"
class SwapChain;
using namespace LibMath;

//struct AllocatedImage
//{
//	VkImage image;
//	VkDeviceMemory allocation;
//	VkImageView imageView;
//};

//class Debug;
class Window;

const std::vector<const char*> g_deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};
class GraphicAPI
{
public:
	static libgfx_API GraphicAPI* GetInstance();


	/**
	* @brief This function create device for Vulkan
	*
	*  @param User window
	*  @param active aliasing
	*/
	libgfx_API void Init(Window* window, bool aliasing);


	libgfx_API void CreateFramebuffers(VkRenderPass* rPass);
	libgfx_API void CreateImGuiFramebuffers(VkRenderPass* rPass);

	libgfx_API void CreateCommandPool();
	libgfx_API void CreateImGuiCommandPool();
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	void RecreateSwapChain(Window* window);
	void DestroyStorageSwapChain();
	[[nodiscard]] bool AliasingIsActive();
	VkSampleCountFlagBits GetMSAASamples();

	//Mutators
	VkInstance& MutVkInstance();
	VkPhysicalDevice& MutPhysicalDevice();
	VkDevice& MutDevice();
	VkRenderPass& MutRenderpass() { return m_renderPass; }
	VkDescriptorSetLayout& MutDescriptorSetLayout() { return m_descriptorSetLayout; }
	VkDescriptorPool& MutDescriptorPool() { return m_descriptorPool; }
	VkDescriptorPool& MutImGuiDescriptorPool() { return m_ImGuiDescriptorPool; }
	std::vector< std::vector<VkDescriptorSet>>& MutDescriptorSets() { return m_descriptorSets; }
	std::stack<size_t>& MutFreeIDModelDescriptorSets() {return m_freeIDModelDescSets;}
	std::stack<size_t>& MutFreeIDTextureDescriptorSets() {return m_freeIDImageDescSets;}
	VkImage& MutTextureImage() { return m_textureImage; }
	VkImageView& MutTextureImageView() { return m_textureImageView; }
	VkDeviceMemory& MutTextureImageMemory() { return m_textureImageMemory; }
	VkPipelineLayout& MutPipelineLayout() { return m_pipelineLayout; }
	VkPipeline& MutGraphicsPipeline() { return m_graphicsPipeline; }
	VkBuffer& MutVertexBuffer() { return m_vertexBuffer; }
	VkDeviceMemory& MutVertexBufferMemory() { return m_vertexBufferMemory; }
	VkBuffer& MutIndexBuffer() { return m_indexBuffer; }
	VkDeviceMemory& MutIndexBufferMemory() { return m_indexBufferMemory; }
	std::vector<std::vector<VkBuffer>>& MutUniformBuffers() { return m_uniformBuffers; }
	std::vector < std::vector<VkDeviceMemory>>& MutUniformBuffersMemory() { return m_uniformBuffersMemory; }
	VkSampler& MutTextureSampler() { return m_textureSampler; }
	std::vector<VkCommandBuffer>& MutCommandBuffers() { return m_commandBuffers; }
	std::vector<VkCommandBuffer>& MutImGuiCommandBuffers() { return m_ImGuiCommandBuffers; }
	std::vector<VkFence>& MutInFlightFences() { return m_inFlightFences; }
	std::vector<VkFence>& MutImagesInFlight() { return m_imagesInFlight; }
	size_t& MutCurrentFrame() { return m_currentFrame; }
	bool& MutIsBufferResized() { return m_bFramebufferResized; }
	std::vector<VkSemaphore>& MutImageAvailableSemaphores() { return m_imageAvailableSemaphores; }
	std::vector<VkSemaphore>& MutRenderFinishedSemaphores() { return m_renderFinishedSemaphores; }
	VkCommandPool& MutCommandPool() { return m_commandPool; }
	VkCommandPool& MutImGuiCommandPool() { return m_ImGuiCommandPool; }
	VkSurfaceKHR& MutSurface() { return m_surface; }
	std::vector<VkFramebuffer>& MutSwapChainFBuffers() { return m_swapChainFramebuffers; }
	std::vector<VkFramebuffer>& MutImGuiFBuffers() { return m_ImGuiFramebuffers; }
	std::vector<VkCommandBuffer>& MutSubmitInfos() { return m_submitCmdBuff; }
	std::vector<std::pair<VkBuffer, VkDeviceMemory>>& MutDeletionQueue() { return m_submitDeletionQueue; }
	SwapChain* GetSwapChain();
	//VkRect2D& MutScreenScissor() { return m_screenScissor; }
	//VkViewport& MutScreenViewport() { return m_screenViewport; }

	//Getters
	const VkInstance GetVkInstance() const { return  m_instanceVK; }
	const VkSurfaceKHR GetSurface() const { return  m_surface; }
	
	const VkPhysicalDevice& GetPhysicalDevice() const { return  m_physicalDevice; }
	const VkDevice GetDevice() const { return  m_device; }

	const VkDescriptorSetLayout GetDescriptorSetLayout() const { return m_descriptorSetLayout; }
	const VkDescriptorPool GetDescriptorPool() const { return m_descriptorPool; }
	const VkDescriptorPool GetImGuiDescriptorPool() const { return m_ImGuiDescriptorPool; }
	const std::vector< std::vector<VkDescriptorSet>>& GetDescriptorSets() const { return m_descriptorSets; }
	const VkImage GetTextureImage() const { return m_textureImage; }
	const VkImageView GetTextureImageView() const { return m_textureImageView; }
	const VkDeviceMemory GetTextureImageMemory() const { return m_textureImageMemory; }
	const VkSampler GetTextureSampler() const { return m_textureSampler; }
	const std::vector<std::vector<VkBuffer>>& GetUniformBuffers() const { return m_uniformBuffers; }
	const std::vector <std::vector<VkDeviceMemory>>& GetUniformBuffersMemory() const { return m_uniformBuffersMemory; }
	
	const VkPipelineLayout GetPipelineLayout() const { return m_pipelineLayout; }
	const VkRenderPass GetRenderpass() const { return m_renderPass; }
	const VkBuffer GetVertexBuffer() const { return m_vertexBuffer; }
	const VkBuffer GetIndexBuffer() const { return m_indexBuffer; }
	const VkDeviceMemory GetVertexBufferMemory() const { return m_vertexBufferMemory; }
	const VkDeviceMemory GetIndexBufferMemory() const { return m_indexBufferMemory; }
	const std::vector<VkFramebuffer>& GetSwapChainFBuffers() const { return m_swapChainFramebuffers; }
	const std::vector<VkFramebuffer>& GetImGuiFBuffers() const { return m_ImGuiFramebuffers; }
	const VkCommandPool GetCommandPool() const { return m_commandPool; }
	const VkCommandPool GetImGuiCommandPool() const { return m_ImGuiCommandPool; }
	const std::vector<VkCommandBuffer>& GetCommandBuffers() const { return m_commandBuffers; }
	const std::vector<VkCommandBuffer>& GetImGuiCommandBuffers() const { return m_ImGuiCommandBuffers; }
	const VkPipeline GetGraphicsPipeline() const { return m_graphicsPipeline; }
	
	const std::vector<VkFence>& GetInFlightFences() const { return m_inFlightFences; }
	const std::vector<VkFence>& GetImagesInFlight() const { return m_imagesInFlight; }
	const size_t GetCurrentFrame() const { return m_currentFrame; }
	const int GetMaxFramesIF() const { return MAX_FRAMES_IN_FLIGHT; }
	const std::vector<VkSemaphore>& GetImageAvailableSemaphores() const { return m_imageAvailableSemaphores; }
	const std::vector<VkSemaphore>& GetRenderFinishedSemaphores() const { return m_renderFinishedSemaphores; }
	const VkQueue GetGraphicsQueue() const { return m_graphicsQueue; }
	const VkQueue GetPresentQueue() const { return m_presentQueue; }
	const bool GetIsBufferResized() const { return m_bFramebufferResized; }
	const VkSampleCountFlagBits& GetSamples() const { return m_msaaSamples; }
	Debug GetDebug() const { return m_debug; }
	//std::vector<AllocatedImage>& GetOffscreenImages();
	//AllocatedImage& GetMSAAImages() { return m_msaaImages; };
	const VkPhysicalDeviceProperties& GetPhysProperties() { return m_properties; }
	const std::vector<VkCommandBuffer>& GetSubmitInfos() { return m_submitCmdBuff; }
	const std::vector<std::pair<VkBuffer, VkDeviceMemory>>& GetDeletionQueue() { return m_submitDeletionQueue; }
	std::mutex& GetSubmitMutex() { return m_mutexSubmit; }

	[[nodiscard]] static VkDeviceSize Align(VkDeviceSize size, VkDeviceSize alignment)
	{
		if (alignment > 0)
			size = (size + alignment - 1) & ~(alignment - 1);

		return size;
	}

	//TODO Should we keep it there? (Libmath include ask for forwarding techniques)
	struct UniformMVP
	{
		alignas(16) Matrix4 VP;
		alignas(16) Matrix4 model;
	};

	struct UniformViewPos
	{
		alignas(16) Vector3 viewPos;
	};

	struct UniformBufferObject
	{
		alignas(16) Matrix4 model;
		alignas(16) Matrix4 view;
		alignas(16) Matrix4 proj;
	};

#pragma warning(push)
#pragma warning(disable : 4324)
	struct LightBufferObject
	{
		alignas(16) Vector3 direction;
		float cutOff;
		float outerCutOff;

		alignas(16) Vector4 position;

		alignas(16) Vector3 ambientColor;
		alignas(16) Vector3 specularColor;
		alignas(16) Vector3 diffuseColor;

		alignas(4) bool bIsPoint;

		float kConstant;
		float kLinear;
		float kQuadratic;

		alignas(16) Vector4 off;
	};
#pragma warning(pop)



	VkCommandBuffer BeginSingleTimeCommands();
	void EndCommandSaveSubmitInfo(VkCommandBuffer commandBuffer, std::pair<VkBuffer, VkDeviceMemory> deletionQueue);
	void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	void CreateSyncObjects();
private:
	/*Singleton*/
	GraphicAPI() = default;
	~GraphicAPI();
	GraphicAPI(GraphicAPI const&) = delete;
	GraphicAPI& operator=(GraphicAPI const&) = delete;

	const int MAX_FRAMES_IN_FLIGHT = 2;

	//VulkanInstance
	VkInstance m_instanceVK;
	VkSurfaceKHR m_surface;

	//VkRect2D m_screenScissor;
	//VkViewport m_screenViewport;
	
	/**
	 *@brief Vulkan Physical device
	 */
	VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
	VkDevice m_device;

	VkQueue m_graphicsQueue;
	VkQueue m_presentQueue;

	SwapChain* m_swapChainClass;

	/*FraMeBuffer imGUI and offscreen*/
	std::vector<VkFramebuffer> m_swapChainFramebuffers;
	std::vector<VkFramebuffer> m_ImGuiFramebuffers;

	VkRenderPass m_renderPass;
	VkDescriptorSetLayout m_descriptorSetLayout;
	VkPipelineLayout m_pipelineLayout;
	VkPipeline m_graphicsPipeline;

	VkCommandPool m_commandPool;
	VkCommandPool m_ImGuiCommandPool;

	VkPhysicalDeviceProperties m_properties{};


	VkImage m_textureImage;
	VkDeviceMemory m_textureImageMemory;
	VkImageView m_textureImageView;
	VkSampler m_textureSampler;

	VkBuffer m_vertexBuffer;
	VkDeviceMemory m_vertexBufferMemory;
	VkBuffer m_indexBuffer;
	VkDeviceMemory m_indexBufferMemory;

	std::vector<std::vector<VkBuffer>> m_uniformBuffers;
	std::vector < std::vector<VkDeviceMemory>> m_uniformBuffersMemory;

	VkDescriptorPool m_descriptorPool;
	VkDescriptorPool m_ImGuiDescriptorPool;
	std::vector<std::vector<VkDescriptorSet>> m_descriptorSets;
	std::stack<size_t> m_freeIDModelDescSets;
	std::stack<size_t> m_freeIDImageDescSets;

	std::vector<VkCommandBuffer> m_commandBuffers;
	std::vector<VkCommandBuffer> m_ImGuiCommandBuffers;

	std::vector<VkSemaphore> m_imageAvailableSemaphores;
	std::vector<VkSemaphore> m_renderFinishedSemaphores;
	std::vector<VkFence> m_inFlightFences;
	std::vector<VkFence> m_imagesInFlight;
	size_t m_currentFrame = 0;

	std::vector<VkCommandBuffer> m_submitCmdBuff;
	std::vector<std::pair<VkBuffer, VkDeviceMemory>> m_submitDeletionQueue;

	bool m_bFramebufferResized = false;

	Debug m_debug;

	std::mutex m_mutexSubmit;

	/**
	 *@brief Number of Samples
	 */
	VkSampleCountFlagBits m_msaaSamples = VK_SAMPLE_COUNT_1_BIT;


	/**
	* @brief Create VK Instance
	*
	*  @param User window
	*/
	void CreateAPIInstance(Window* window);

	/**
	*
	*  @brief Init Set callback debug messenger
	*/
	void SetupVulkanDebugCallback();

	/**
	* @brief This function creates a Vulkan surface for the specified window.
	*
	*  @param User window
	*/
	void CreateWindowSurface(Window* window);

	/**
	* @brief This function search and set Physical device
	*
	*/
	void PickPhysicalDevice();

	/**
	* @brief This function create device for Vulkan
	*
	*/
	void CreateLogicalDevice();


	/*--------Init utility functions---------*/
	/**
	* @brief This function returns an array of names of Vulkan instance extensions required by GLFW
	*
	* @param return all Extensions
	*/
	std::vector<const char*> GetRequiredExtensions(Window* window);

	/**
	* @brief Checks that all the validation layers are present in the list of available layers
	*/
	bool CheckValidationLayerSupport();


	/**
	* @brief Search for ideal GPU. Ensure that the physical m_device can receive the commands we want to send to it
	*
	*  @param Current device
	*/
	bool IsDeviceSuitable(VkPhysicalDevice device);

	/**
	* @brief Find the family indices to add to the structure
	*
	*  @param Current device
	*/
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);

	/**
	* @brief Enumerate extensions and check if all required extensions are included
	*
	*  @param Current device
	*/
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);

	/**
	 *@brief Determining how many samples our hardware can use
	 */
	VkSampleCountFlagBits GetMaxUsableSampleCount();
	
	/**
	 *@brief bool check if aliasing is activate or no
	 */
	bool m_bAliasing = false;
};
