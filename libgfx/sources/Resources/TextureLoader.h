#pragma once
#include "libgfx_Export.h"

#include <cstdint>
#include <string_view>

class Image;

struct libgfx_API TemporaryTexture
{
	unsigned char* pixels;
	TemporaryTexture(const char* filename, int* width, int* height, int* comp, int req_comp);
	TemporaryTexture(const TemporaryTexture& other) = delete;
	TemporaryTexture& operator=(const TemporaryTexture&& other) = delete;
		
	~TemporaryTexture();
};

struct DataTexture
{
public:
	enum ImageFormat
	{
		INVALID,
		R,
		RG,
		RGB,
		RGBA
	};

	int width, height, depth;
	uint32_t layer_count;
	uint32_t mip_levels;
	uint32_t array_layers;
	unsigned char* texels;
	//std::unique_ptr<stbi_uc, decltype(&free)> texels{ nullptr,&free };
	int block_size;
	ImageFormat format;
	static Image CreateTexture(std::string_view filename, ImageFormat format, bool use_mipmaps);

	[[nodiscard]] size_t Size() const;

private:
	static int GetRequiredChannels(ImageFormat format);
};
