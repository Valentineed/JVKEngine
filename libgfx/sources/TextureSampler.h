#pragma once

#include  "GraphicAPI.h"

struct Sampler
{
	enum Filter
	{
		Linear,
		Nearest
	};

	VkSampler sampler;
	Filter filterType;

	static Sampler CreateSampler(Filter filter)
	{
		Sampler sampler;

		sampler.filterType = filter;
		sampler.CreateTextureSampler(filter);

		return sampler;
	}

private:
	void CreateTextureSampler(Filter filter)
	{
		GraphicAPI* api = GraphicAPI::GetInstance();

		VkSamplerCreateInfo samplerInfo{};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = FilterConvert(filter);
		samplerInfo.minFilter = FilterConvert(filter);
		/*VK_SAMPLER_ADDRESS_MODE_REPEAT : répète la texture
		VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT : répète en inversant les coordonnées pour réaliser un effet miroir
		VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE : prend la couleur du pixel de bordure le plus proche
		VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE : prend la couleur de l'opposé du plus proche côté de l'image
		VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER : utilise une couleur fixée*/
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;									//Use of anistropic filtering
		samplerInfo.maxAnisotropy = api->GetPhysProperties().limits.maxSamplerAnisotropy;		// Max number of texels used to compute a color (Max commonly == 16.0f) (//TODO Reduce if lacks perf)
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;			//VK_TRUE -> Coordinates in [0, texWidth) and [0, texHeight) ... VK_FALSE -> [0, 1);
		samplerInfo.compareEnable = VK_FALSE;					//Use for percentage-closer filtering
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;

		if (vkCreateSampler(api->GetDevice(), &samplerInfo, nullptr, &sampler) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create texture sampler!");
		}
	}

	VkFilter FilterConvert(Filter samplerFilter)
	{
		switch (samplerFilter)
		{
		case Linear:
			return VK_FILTER_LINEAR;
			break;
		case Nearest:
			return VK_FILTER_NEAREST;
			break;
		}
		return VkFilter{};
	}
};