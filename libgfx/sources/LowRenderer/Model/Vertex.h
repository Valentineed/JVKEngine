#pragma once
#include <vulkan\vulkan.h>

#include "Vector/Vector.h"

#include <array>

using namespace LibMath;

struct Vertex
{
	Vector3 pos;
	Vector3 normal;
	TVector2<float> texCoord;
	//Assume texture id = 0 == default_texture

	//Todo review
	//A vertex binding describes the reading of data stored in memory
	static VkVertexInputBindingDescription GetBindingDescription()
	{
		VkVertexInputBindingDescription bindingDescription{};
		bindingDescription.binding = 0;
		bindingDescription.stride = sizeof(Vertex);
		bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		return bindingDescription;
	}

	//Todo review
	/*They each describe the origin and nature of the data stored in a shader variable annotated with location = x,
	 *and how to determine its values from the data extracted by the binding. Since we have two of these variables,
	 *we need two of these structures.*/
	static std::array<VkVertexInputAttributeDescription, 3> GetAttributeDescriptions()
	{
		std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};

		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[0].offset = offsetof(Vertex, pos);

		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
		attributeDescriptions[1].offset = offsetof(Vertex, normal);

		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
		attributeDescriptions[2].offset = offsetof(Vertex, texCoord);
		
		return attributeDescriptions;
	}
};