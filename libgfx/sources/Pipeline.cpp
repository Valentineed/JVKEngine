#include "Pipeline.h"
#include "Core/SwapChain.h"

void Pipeline::CreateGraphicsPipeline(const VkVertexInputBindingDescription& bindingDescriptions, VkVertexInputAttributeDescription& attributeDescriptions)
{
	GraphicAPI* api = GraphicAPI::GetInstance();

	VkPipelineShaderStageCreateInfo shaderStages[] = { shaderVert.shaderStageCreateInfo, shaderFrag.shaderStageCreateInfo };

	//Describes the format of the vertices sent to the vertex shader
	VkPipelineVertexInputStateCreateInfo vertexInputInfo = Initializers::pipelineVertexInputStateCreateInfo();
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescriptions;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(Vertex::GetAttributeDescriptions().size());
	vertexInputInfo.pVertexAttributeDescriptions = &attributeDescriptions;

	//Describes the nature of the desired geometry when the vertices are connected, and allows to activate or not the reevaluation of the vertices
	VkPipelineInputAssemblyStateCreateInfo inputAssembly = Initializers::pipelineInputAssemblyStateCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, VK_FALSE);
	/*VK_PRIMITIVE_TOPOLOGY_POINT_LIST: each vertex is a point
	VK_PRIMITIVE_TOPOLOGY_LINE_LIST: draws a line connecting two vertices using them only once
	VK_PRIMITIVE_TOPOLOGY_LINE_STRIP: the last vertex of each row is used as the first vertex for the next row
	VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST: draw a triangle using three vertices, without reusing any for the next triangle
	VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP: the second and third vertices are used as the first two for the following triangle*/

	VkViewport viewport = Initializers::viewport(static_cast<float>(api->GetSwapChain()->GetSwapChainExtent().width), static_cast<float>(api->GetSwapChain()->GetSwapChainExtent().height), 0.0f, 1.0f);

	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	//TODO: Maybe remove the hard coding and find better scaling values to cut/place the viewport
	viewport.x = 0.f;
	viewport.y = 0.f;

	//Scissor cut all out of range
	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = api->GetSwapChain()->GetSwapChainExtent();

	//https://www.khronos.org/registry/vulkan/specs/1.2-extensions/man/html/VkPipelineViewportStateCreateInfo.html
	//Structure specifying parameters of a newly created pipeline viewport state
	VkPipelineViewportStateCreateInfo viewportState = Initializers::pipelineViewportStateCreateInfo(1, 1, 0);
	viewportState.pViewports = &viewport;
	viewportState.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer = Initializers::pipelineRasterizationStateCreateInfo(VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE, 0);
	//VK_POLYGON_MODE_FILL: fill polygons with fragments
	//VK_POLYGON_MODE_LINE: the sides of polygons are drawn as lines
	//VK_POLYGON_MODE_POINT: vertices are drawn as points

	VkPipelineMultisampleStateCreateInfo multisampling = Initializers::pipelineMultisampleStateCreateInfo(api->GetSamples(), VK_TRUE, 0);
	multisampling.rasterizationSamples = api->GetSamples();

	VkPipelineDepthStencilStateCreateInfo depthStencil = Initializers::pipelineDepthStencilStateCreateInfo(VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL);
	depthStencil.depthBoundsTestEnable = VK_FALSE;
	depthStencil.stencilTestEnable = VK_FALSE;

	//0xf == {VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT}?
	VkPipelineColorBlendAttachmentState colorBlendAttachment = Initializers::pipelineColorBlendAttachmentState(0xf, VK_FALSE);

	VkPipelineColorBlendStateCreateInfo colorBlending = Initializers::pipelineColorBlendStateCreateInfo(1, &colorBlendAttachment, VK_FALSE);
	//colorBlending.blendConstants[0] = 0.0f;
	//colorBlending.blendConstants[1] = 0.0f;
	//colorBlending.blendConstants[2] = 0.0f;
	//colorBlending.blendConstants[3] = 0.0f;

	VkPipelineLayoutCreateInfo pipelineLayoutInfo = Initializers::pipelineLayoutCreateInfo(shaderDSetLayouts.data(), static_cast<uint32_t>(shaderDSetLayouts.size()));
	//VkPipelineLayoutCreateInfo pipelineLayoutInfo = Initializers::pipelineLayoutCreateInfo(&shaderDescSetLayout.setLayout, 1);

	////TODO Care to change this if the pushConstant type changes.
	//VkPushConstantRange pushConstantRange = Initializers::pushConstantRange(VK_SHADER_STAGE_VERTEX_BIT, sizeof(Matrix4), 0);
	//pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
	//pipelineLayoutInfo.pushConstantRangeCount = 1;

	if (vkCreatePipelineLayout(api->GetDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create pipeline layout!");
	}

	VkGraphicsPipelineCreateInfo pipelineInfo = Initializers::pipelineCreateInfo(pipelineLayout, renderPass, 0);
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = 2;
	pipelineInfo.pStages = shaderStages;
	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &depthStencil;
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.subpass = 0;
	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

	if (vkCreateGraphicsPipelines(api->GetDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create graphics pipeline!");
	}

	//TODO check memory leak
	shaderVert.DestroyShaderModule();
	shaderFrag.DestroyShaderModule();
}
