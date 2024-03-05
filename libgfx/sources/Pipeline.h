#pragma once

#include "GraphicAPI.h"
#include "Descriptors.h"
#include "LowRenderer/Model/Vertex.h"
#include "Common.h"
#include "Shader.h"
#include "Initializers.hpp"

struct Pipeline
{
	VkRenderPass renderPass;
	VkPipeline graphicsPipeline;
	VkPipelineLayout pipelineLayout;
	//DescriptorSetLayout shaderDescSetLayout;	//TODO rm
	std::vector<VkDescriptorSetLayout> shaderDSetLayouts;

	/**
	* @brief Vertex shader
	*/
	Shader shaderVert;

	/**
	* @brief ShaderFrag is pixel shader
	*/
	Shader shaderFrag;

	void CreateGraphicsPipeline(const VkVertexInputBindingDescription& bindingDescriptions, VkVertexInputAttributeDescription& attributeDescriptions);

	static Pipeline CreatePipeline(const std::string& vertFilename, const std::string& fragmentFilename, const std::vector<DescriptorSetLayout>& descSetLayout, VkRenderPass* rPass)
	{
		Pipeline pipeline;

		pipeline.renderPass = *rPass;

		pipeline.shaderVert = Shader::LoadVertexShader(vertFilename);
		pipeline.shaderFrag = Shader::LoadFragmentShader(fragmentFilename);

		
		for (auto& setLayout : descSetLayout)
		{
			pipeline.shaderDSetLayouts.push_back(setLayout.setLayout);
		}

		pipeline.CreateGraphicsPipeline(Vertex::GetBindingDescription(), *Vertex::GetAttributeDescriptions().data());

		return pipeline;
	}
};
