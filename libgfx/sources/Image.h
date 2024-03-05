#pragma once
#include "GraphicAPI.h"
#include "Resources/SavedData.h"

struct DataTexture;
struct DescriptorSetLayout;
struct Sampler;

class Image : public SavedResource
{
public:
	VkImage m_image;
	VkImageView m_imageView;			//Texture factory -> vector<VkImageView>
	size_t m_ID;
	VkDeviceMemory m_imageMemory;

	Image() = default;
	Image(const Image& other) :
		m_image(other.m_image), m_imageView(other.m_imageView), m_imageMemory(other.m_imageMemory)
	{
	}

	void DestroyImage() const;
	void DestroyImage(size_t setID) const;

	void CreateImage2D(const DataTexture& data, Image& img);

private:
	void CreateTextureImage(const DataTexture& dataImg, VkImageViewType viewType);
	void CreateImage(uint32_t width, uint32_t height, VkSampleCountFlagBits numSample,VkFormat format, VkImageType imgType, VkImageTiling tiling,
		VkImageUsageFlags usage, VkMemoryPropertyFlags properties);
	void CreateImageView(VkImageViewType imgViewType, VkFormat format, VkImageAspectFlags aspectFlags);
	void TransitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout, VkCommandBuffer transiBuffer);
	void CopyBufferToImage(VkBuffer buffer, uint32_t width, uint32_t height, VkCommandBuffer transiBuffer);

	VkFormat GetVkFormat(int format);
	VkImageType GetImgFromView(VkImageViewType viewType);
};

struct SavedTexture
{
	Image image;
	size_t ID;
	size_t setID;

	SavedTexture() = delete;
	SavedTexture(const Image& img, size_t id) :
		image(img), ID(id)
	{
	}
	~SavedTexture() = default;

	void Destroy()
	{
		image.DestroyImage(setID);
		this->~SavedTexture();
	}

	void CreateDescriptorSet(DescriptorSetLayout& setLayout);
	void UpdateDescriptorSet(VkImageView imgView, VkSampler& sampler);
};
