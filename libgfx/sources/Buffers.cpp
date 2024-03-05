#include "Buffers.h"

#include "Core/SwapChain.h"

void DataBuffers::CreateUniformBuffers()
{
	GraphicAPI* api = GraphicAPI::GetInstance();

	VkDeviceSize bufferSize = sizeof(GraphicAPI::UniformBufferObject);

	const size_t swapChainImgCount = api->GetSwapChain()->ImageCount();
	api->MutUniformBuffers().resize(swapChainImgCount);
	api->MutUniformBuffersMemory().resize(swapChainImgCount);

	for (size_t i = 0; i < swapChainImgCount; i++)
	{
		//TODO CARE TO RESIZE WITH NO HARDVALUES
		api->MutUniformBuffers()[i].resize(2);
		api->MutUniformBuffersMemory()[i].resize(2);

		for (size_t j = 0; j < api->MutUniformBuffers()[i].size(); j++)
		{
			CreateBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, api->MutUniformBuffers()[i][j], api->MutUniformBuffersMemory()[i][j]);
		}
	}
}
