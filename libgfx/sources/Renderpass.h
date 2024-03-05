#pragma once
#include <vulkan/vulkan_core.h>


class RenderPass
{
public:
	/**
	* @brief Get a Vulkan RenderPass
	* @return a pointer in VkRenderPass.
	*/
	VkRenderPass* GetRenderPass();

	/**
	* @brief Create a common RenderPass
	*/
	static RenderPass CreateRenderpass();

	/**
	* @brief Create RenderPass for IMGUI
	*/
	static RenderPass CreateImGuiRenderPass();
private:
	/**
	* @brief Storage a RenderPass
	*/
	VkRenderPass m_renderPass = nullptr;

	/**
	* @brief Create a common MSAA RenderPass
	*/
	static RenderPass CreateMSAARenderpass();
	
	/**
	* @brief Create a common MSAA RenderPass
	*/
	static RenderPass CreateCommonRenderpass();
};
