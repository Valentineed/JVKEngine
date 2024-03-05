#pragma warning(push)
#pragma warning(disable: 26451)
#pragma warning(disable: 6308)
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#pragma warning(pop)
#include "TextureLoader.h"
#include "Image.h"

#include <algorithm>
#include <cmath>

TemporaryTexture::TemporaryTexture(const char* filename, int* width, int* height, int* comp, int req_comp)
{
	pixels = stbi_load(filename, width, height, comp, req_comp);

	if(pixels == nullptr)
	{
		throw("Failed to load texture\n");
	}}

TemporaryTexture::~TemporaryTexture()
{
	stbi_image_free(pixels);
}

Image DataTexture::CreateTexture(std::string_view filename, ImageFormat format, bool use_mipmaps)
{
	DataTexture data{};
	int nbrChannels;
	const TemporaryTexture temp(filename.data(), &data.width, &data.height, &nbrChannels, GetRequiredChannels(format));
	data.depth = 1;
	data.texels = temp.pixels;
	data.mip_levels = !use_mipmaps ? 1 : (uint32_t)std::log2(std::max(data.width, data.height)) + 1;
	data.array_layers = 1;
	data.block_size = format;
	data.format = format;
	Image img;
	img.CreateImage2D(data, img);
	return img;
}
size_t DataTexture::Size() const
{
	return (size_t)width * (size_t)height * (size_t)depth * (size_t)block_size;
}

int DataTexture::GetRequiredChannels(ImageFormat format)
{
	switch (format)
	{
	case R:
		return STBI_grey;
		break;
	case RG:
		return STBI_grey_alpha;
		break;
	case RGB:
		return STBI_rgb;
		break;
	case RGBA:
		return STBI_rgb_alpha;
		break;
	}
	return 0;
}
