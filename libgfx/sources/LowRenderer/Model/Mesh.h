#pragma once
#include "Vertex.h"
#include "Buffers.h"

struct Mesh
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	DataBuffers vertexBuffer;
	DataBuffers indexBuffer;
	
	std::vector<VkDeviceSize> offsets{ 0 };

	Mesh() = default;
	Mesh(std::vector<Vertex> vert, std::vector<uint32_t> ind) :
		vertices(std::move(vert)), indices(std::move(ind))
	{
	}

	void Draw(const VkCommandBuffer& commandBuffer, const VkPipelineLayout pipelineLayout, VkDescriptorSet& descriptorSet)
	{
		vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer.buffer, offsets.data());

		vkCmdBindIndexBuffer(commandBuffer, indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT32);

		vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 3, 1, &descriptorSet, 0, nullptr);

		vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
	}
};
