#pragma once

#include "TextureLoader.h"
#include "TextureSampler.h"
#include "Image.h"

#include <string>
#include <imgui_impl_glfw.h>

class Image;

class FileAsset
{
public:
	FileAsset(std::string path, std::string projectPath, Sampler& sampler);
	std::string GetName();
	std::string GetPath();
	std::string GetExtension();
	std::string GetNameWithExt();
	std::string GetDisplayName();
	void SetDisplayName(std::string displayName);

	ImTextureID& GetTextureID();

	void DestroyImage();

	void DestroyDefaultImage();

	bool IsOnDefaultTexture();

	int m_numberOfDuplicates = 0;
private:
	std::string m_name;
	std::string m_extension;
	std::string m_path;
	std::string m_nameWithExt;
	std::string m_displayName;

	DataTexture m_thumbnailData;

	ImTextureID m_textureID;

	Image m_thumbnailImage;

	Image m_defaultNonSupportedFileExtImage;

	bool m_bIsOnDefaultTexture = true;

	
};
