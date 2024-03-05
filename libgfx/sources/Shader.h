#pragma once

#include "GraphicAPI.h"
#include "Common.h"
#include "vulkan/vulkan.h"

#include <string>

//todo wrappe this enum?
/*typedef enum VkShaderStageFlagBits {
	VK_SHADER_STAGE_VERTEX_BIT = 0x00000001,
	VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT = 0x00000002,
	VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT = 0x00000004,
	VK_SHADER_STAGE_GEOMETRY_BIT = 0x00000008,
	VK_SHADER_STAGE_FRAGMENT_BIT = 0x00000010,
	VK_SHADER_STAGE_COMPUTE_BIT = 0x00000020,
	VK_SHADER_STAGE_ALL_GRAPHICS = 0x0000001F,
	VK_SHADER_STAGE_ALL = 0x7FFFFFFF,
	VK_SHADER_STAGE_RAYGEN_BIT_KHR = 0x00000100,
	VK_SHADER_STAGE_ANY_HIT_BIT_KHR = 0x00000200,
	VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR = 0x00000400,
	VK_SHADER_STAGE_MISS_BIT_KHR = 0x00000800,
	VK_SHADER_STAGE_INTERSECTION_BIT_KHR = 0x00001000,
	VK_SHADER_STAGE_CALLABLE_BIT_KHR = 0x00002000,
	VK_SHADER_STAGE_TASK_BIT_NV = 0x00000040,
	VK_SHADER_STAGE_MESH_BIT_NV = 0x00000080,
	VK_SHADER_STAGE_RAYGEN_BIT_NV = VK_SHADER_STAGE_RAYGEN_BIT_KHR,
	VK_SHADER_STAGE_ANY_HIT_BIT_NV = VK_SHADER_STAGE_ANY_HIT_BIT_KHR,
	VK_SHADER_STAGE_CLOSEST_HIT_BIT_NV = VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR,
	VK_SHADER_STAGE_MISS_BIT_NV = VK_SHADER_STAGE_MISS_BIT_KHR,
	VK_SHADER_STAGE_INTERSECTION_BIT_NV = VK_SHADER_STAGE_INTERSECTION_BIT_KHR,
	VK_SHADER_STAGE_CALLABLE_BIT_NV = VK_SHADER_STAGE_CALLABLE_BIT_KHR,
	VK_SHADER_STAGE_FLAG_BITS_MAX_ENUM = 0x7FFFFFFF
} VkShaderStageFlagBits;*/

struct Shader
{
	VkPipelineShaderStageCreateInfo shaderStageCreateInfo;
	VkShaderModule shaderModule;
	static Shader Load(VkShaderStageFlagBits shaderStage, const std::string& filename);

	void DestroyShaderModule()
	{
		vkDestroyShaderModule(GraphicAPI::GetInstance()->GetDevice(), shaderModule, nullptr);
	}

	static Shader LoadVertexShader(const std::string& filename);
	static Shader LoadFragmentShader(const std::string& filename);
};

//todo create .inl?
inline Shader Shader::Load(VkShaderStageFlagBits shaderStage, const std::string& filename)
{
	std::vector<char> shaderData = ReadFile(filename);
	Shader shader;

	VkShaderModuleCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = (uint32_t)shaderData.size();
	createInfo.pCode = reinterpret_cast<const uint32_t*>(shaderData.data());//transfer the char in uint32_t

	///CHECK_VK_RESULT(vkCreateShaderModule(GraphicAPI::GetInstance()->MutDevice(), &createInfo, nullptr, &shaderModule/*shader.shader_piece.replace()*/), );
	if (vkCreateShaderModule(GraphicAPI::GetInstance()->GetDevice(), &createInfo, nullptr, &shader.shaderModule) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create shader module!");
	}

	//Assign a shader step to modules
	shader.shaderStageCreateInfo = {};
	shader.shaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	shader.shaderStageCreateInfo.stage = shaderStage;
	shader.shaderStageCreateInfo.module = shader.shaderModule;
	shader.shaderStageCreateInfo.pName = "main";

	return shader;
}

inline Shader Shader::LoadVertexShader(const std::string& filename)
{
	return Load(VK_SHADER_STAGE_VERTEX_BIT, filename);
}

inline Shader Shader::LoadFragmentShader(const std::string& filename)
{
	return Load(VK_SHADER_STAGE_FRAGMENT_BIT, filename);
}