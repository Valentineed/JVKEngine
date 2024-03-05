#include "Image.h"
#include "Buffers.h"
#include "Resources/TextureLoader.h"
#include "Descriptors.h"
#include "Core/SwapChain.h"

void Image::DestroyImage() const
{
	GraphicAPI* api = GraphicAPI::GetInstance();

	vkDestroyImage(api->GetDevice(), m_image, nullptr);
	vkDestroyImageView(api->GetDevice(), m_imageView, nullptr);
	vkFreeMemory(api->GetDevice(), m_imageMemory, nullptr);
}

void Image::DestroyImage(size_t setID) const
{
	GraphicAPI* api = GraphicAPI::GetInstance();

	api->MutFreeIDTextureDescriptorSets().push(setID);
	vkDestroyImage(api->GetDevice(), m_image, nullptr);
	vkDestroyImageView(api->GetDevice(), m_imageView, nullptr);
	vkFreeMemory(api->GetDevice(), m_imageMemory, nullptr);
}

void Image::CreateImage2D(const DataTexture& data, Image& img)
{
	img.CreateTextureImage(data, VK_IMAGE_VIEW_TYPE_2D);
}

void Image::CreateTextureImage(const DataTexture& dataImg, VkImageViewType viewType)
{
	GraphicAPI* api = GraphicAPI::GetInstance();

	VkImageType imgType = GetImgFromView(viewType);
	VkDeviceSize imgSize = dataImg.Size();
	DataBuffers imgBuffer;

	//Staging Buffer
	imgBuffer.CreateBuffer(imgSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

	//VkImageSubresourceLayers image_subresource{};
	//image_subresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	//image_subresource.mipLevel = 0;
	//image_subresource.baseArrayLayer = 0;
	//image_subresource.layerCount = 1;

	void* data;
	vkMapMemory(api->GetDevice(), imgBuffer.bufferMemory, 0, imgSize, 0, &data);
	memcpy(data, dataImg.texels, static_cast<size_t>(imgSize));
	vkUnmapMemory(api->GetDevice(), imgBuffer.bufferMemory);

	VkFormat format = GetVkFormat((int)dataImg.format);
	CreateImage(dataImg.width, dataImg.height, VK_SAMPLE_COUNT_1_BIT,format, imgType, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	VkCommandBuffer buff = api->BeginSingleTimeCommands();
	TransitionImageLayout(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, buff);
	CopyBufferToImage(imgBuffer.buffer, static_cast<uint32_t>(dataImg.width), static_cast<uint32_t>(dataImg.height), buff);
	TransitionImageLayout(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, buff);
	api->EndCommandSaveSubmitInfo(buff, std::make_pair(imgBuffer.buffer, imgBuffer.bufferMemory));
	CreateImageView(viewType, format, VK_IMAGE_ASPECT_COLOR_BIT);

	//vkDestroyBuffer(api->GetDevice(), imgBuffer.buffer, nullptr);
	//vkFreeMemory(api->GetDevice(), imgBuffer.bufferMemory, nullptr);
}

void Image::CreateImage(uint32_t width, uint32_t height, VkSampleCountFlagBits numSample ,VkFormat format, VkImageType imgType, VkImageTiling tiling,
                        VkImageUsageFlags usage, VkMemoryPropertyFlags properties)
{
	GraphicAPI* api = GraphicAPI::GetInstance();

	VkImageCreateInfo imageInfo{};
	imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imageInfo.imageType = imgType;
	imageInfo.extent.width = width;
	imageInfo.extent.height = height;
	imageInfo.extent.depth = 1;
	imageInfo.mipLevels = 1;
	imageInfo.arrayLayers = 1;
	imageInfo.format = format;
	imageInfo.tiling = tiling;
	imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	imageInfo.usage = usage;
	imageInfo.samples = numSample;
	imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	if (vkCreateImage(api->GetDevice(), &imageInfo, nullptr, &m_image) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create image!");
	}

	VkMemoryRequirements memRequirements;
	vkGetImageMemoryRequirements(api->GetDevice(), m_image, &memRequirements);

	VkMemoryAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memRequirements.size;
	allocInfo.memoryTypeIndex = api->FindMemoryType(memRequirements.memoryTypeBits, properties);

	if (vkAllocateMemory(api->GetDevice(), &allocInfo, nullptr, &m_imageMemory) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to allocate image memory!");
	}

	vkBindImageMemory(api->GetDevice(), m_image, m_imageMemory, 0);
}

void Image::CreateImageView(VkImageViewType imgViewType, VkFormat format, VkImageAspectFlags aspectFlags)
{
	GraphicAPI* api = GraphicAPI::GetInstance();

	VkImageViewCreateInfo viewInfo{};
	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = m_image;
	viewInfo.viewType = imgViewType;
	viewInfo.format = format;

	viewInfo.subresourceRange.aspectMask = aspectFlags;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;
	if (vkCreateImageView(api->GetDevice(), &viewInfo, nullptr, &m_imageView) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create texture image view!");
	}
}

void Image::TransitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout, VkCommandBuffer transiBuffer)
{
	//GraphicAPI* api = GraphicAPI::GetInstance();

	//VkCommandBuffer commandBuffer = api->BeginSingleTimeCommands();

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.oldLayout = oldLayout; //VK_IMAGE_LAYOUT_UNDEFINED if not interested in the data of this image
	barrier.newLayout = newLayout;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED; //Ought to send them the family queues indexes
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = m_image; //Image targetted by the modfications
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	//SubresourceRange -> Part of the image targetted by the modifications
	barrier.subresourceRange.baseMipLevel = 0;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;

	VkPipelineStageFlags sourceStage;
	VkPipelineStageFlags destinationStage;

	if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
	{
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

		sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
		destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
	}
	else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
	{
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	}
	else
	{
		throw std::invalid_argument("unsupported layout transition!");
	}

	vkCmdPipelineBarrier(
		transiBuffer,
		sourceStage, destinationStage,
		0,
		0, nullptr,
		0, nullptr,
		1, &barrier
	);

	//api->EndSingleTimeCommands(commandBuffer);
}

void Image::CopyBufferToImage(VkBuffer buffer, uint32_t width, uint32_t height, VkCommandBuffer transiBuffer)
{
	//GraphicAPI* api = GraphicAPI::GetInstance();

	//VkCommandBuffer commandBuffer = api->BeginSingleTimeCommands();

	VkBufferImageCopy region{};
	region.bufferOffset = 0;
	region.bufferRowLength = 0;
	region.bufferImageHeight = 0;
	region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.imageSubresource.mipLevel = 0;
	region.imageSubresource.baseArrayLayer = 0;
	region.imageSubresource.layerCount = 1;
	region.imageOffset = {0, 0, 0};
	region.imageExtent = {
		width,
		height,
		1
	};

	vkCmdCopyBufferToImage(transiBuffer, buffer, m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

	//api->EndSingleTimeCommands(commandBuffer);
}

VkFormat Image::GetVkFormat(int format)
{
	switch (format)
	{
	case DataTexture::ImageFormat::R:
		return VK_FORMAT_R8_UNORM;
		break;
	case DataTexture::ImageFormat::RG:
		return VK_FORMAT_R8G8_UNORM;
		break;
	case DataTexture::ImageFormat::RGB:
		return VK_FORMAT_R8G8B8_UNORM;
		break;
	case DataTexture::ImageFormat::RGBA:
		return VK_FORMAT_R8G8B8A8_UNORM;
		break;
	}
	return VK_FORMAT_UNDEFINED;
}

VkImageType Image::GetImgFromView(VkImageViewType viewType)
{
	if (viewType == VK_IMAGE_VIEW_TYPE_1D || viewType == VK_IMAGE_VIEW_TYPE_1D_ARRAY)
	{
		return VK_IMAGE_TYPE_1D;
	}
	if (viewType == VK_IMAGE_VIEW_TYPE_2D || viewType == VK_IMAGE_VIEW_TYPE_2D_ARRAY)
	{
		return VK_IMAGE_TYPE_2D;
	}
	if (viewType == VK_IMAGE_VIEW_TYPE_3D || viewType == VK_IMAGE_VIEW_TYPE_CUBE || viewType ==
		VK_IMAGE_VIEW_TYPE_CUBE_ARRAY)
	{
		return VK_IMAGE_TYPE_3D;
	}
	return VK_IMAGE_TYPE_MAX_ENUM;
}

void SavedTexture::CreateDescriptorSet(DescriptorSetLayout& setLayout)
{
	GraphicAPI* api = GraphicAPI::GetInstance();

	//--------------------------------------------------------------------Test--------------------------------------------------------------------

	//VkDescriptorSetVariableDescriptorCountAllocateInfoEXT variableDescriptorCountAllocInfo = {};

	//uint32_t variableDescCounts[] = { static_cast<uint32_t>(textures.size()) };

	//variableDescriptorCountAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_VARIABLE_DESCRIPTOR_COUNT_ALLOCATE_INFO_EXT;
	//variableDescriptorCountAllocInfo.descriptorSetCount = 1;
	//variableDescriptorCountAllocInfo.pDescriptorCounts = variableDescCounts;

	//--------------------------------------------------------------------------------------------------------------------------------------------
	//Same nb of layout as descriptorSetCount
	const size_t swapChainImageCount = api->GetSwapChain()->ImageCount();
	std::vector<VkDescriptorSetLayout> layouts(swapChainImageCount, setLayout.setLayout);
	VkDescriptorSetAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = api->GetDescriptorPool();	//If using Pool of pool method, need to dynamically swap descpPool
	allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImageCount);
	allocInfo.pSetLayouts = layouts.data();

	//--------------------------------------------------------------------Test--------------------------------------------------------------------

	//allocInfo.pNext = &variableDescriptorCountAllocInfo;

	//--------------------------------------------------------------------Test--------------------------------------------------------------------

	//if (models.size() > 0)
	//{
	//	for (size_t iMod = 0; iMod < models.size() - 1; iMod++)
	//	{
	//		if (models[iMod].matID != static_cast<size_t>(-1) && models[iMod].matID == models[models.size() - 1].matID)
	//		{
	//			return;
	//		}
	//	}
	//}
	if (api->MutFreeIDTextureDescriptorSets().empty())
	{
		std::vector<VkDescriptorSet>* NewSet = __nullptr;

		NewSet = &api->MutDescriptorSets().emplace_back();
		setID = api->GetDescriptorSets().size() - 1;
		NewSet->resize(swapChainImageCount);

		if (vkAllocateDescriptorSets(api->GetDevice(), &allocInfo, (*NewSet).data()) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate descriptor sets!");
		}
	}
	else
	{
		setID = api->MutFreeIDTextureDescriptorSets().top();
		api->MutFreeIDTextureDescriptorSets().pop();
	}
}

void SavedTexture::UpdateDescriptorSet(VkImageView imgView, VkSampler& sampler)
{
	GraphicAPI* api = GraphicAPI::GetInstance();

	for (int iSet = 0; iSet < api->GetDescriptorSets()[setID].size(); iSet++)
	{
		//std::vector<VkWriteDescriptorSet> descriptorWrites;
		
		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.sampler = sampler;
		imageInfo.imageView = imgView;
		
		//if (textures.size() > 1 && textureID > 0)
		//{
		//	imageInfo.imageView = textures[textureID].image.imageView;
		//}
		//else
		//{
		//	//Assume that id 0 is always defaultTexture
		//	imageInfo.imageView = textures[0].image.imageView;
		//}

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = api->GetDescriptorSets()[setID][iSet];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pImageInfo = &imageInfo;

		//descriptorWrites.push_back(descriptorWrite);

		vkUpdateDescriptorSets(api->GetDevice(), 1, &descriptorWrite, 0, nullptr);
	}
}
