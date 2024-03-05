#include "FileAsset.h"
#include "imgui_impl_vulkan.h"

#include <filesystem>

FileAsset::FileAsset(std::string path, std::string projectPath, Sampler& sampler) : m_path(path)
{
	std::filesystem::path fileNotSupportedTexturePath = projectPath;
	fileNotSupportedTexturePath /= "resources";
	fileNotSupportedTexturePath /= "UI/Icons";
	fileNotSupportedTexturePath /= "file_not_supported.jpg";
	m_defaultNonSupportedFileExtImage = m_thumbnailData.CreateTexture(fileNotSupportedTexturePath.string(), DataTexture::RGBA, false);

	//m_textureID = ImGui_ImplVulkan_AddTexture(sampler.sampler, m_thumbnailImage.imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	std::size_t indexDot = path.find_last_of('.');
	if (indexDot != std::string::npos && indexDot == path.length() - 4)
	{
		m_extension = path.substr(indexDot + 1);
		path.erase(indexDot, m_extension.length() + 1);
		std::size_t indexLastSlash = path.find_last_of('\\');
		m_name = path.substr(indexLastSlash + 1);
	}

	//TODO : Add extension TGA after
	if ((m_extension == "jpg" || m_extension == "png" || m_extension == "tga") && !m_path.empty())
	{
		m_bIsOnDefaultTexture = false;
		m_thumbnailImage = m_thumbnailData.CreateTexture(m_path.c_str(), DataTexture::RGBA, false);

		m_textureID = ImGui_ImplVulkan_AddTexture(sampler.sampler, m_thumbnailImage.m_imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}
	else if (m_extension == "xml")
	{
		m_bIsOnDefaultTexture = false;
		m_thumbnailImage = m_thumbnailData.CreateTexture("resources/images/FileLevel.png", DataTexture::RGBA, false);

		m_textureID = ImGui_ImplVulkan_AddTexture(sampler.sampler, m_thumbnailImage.m_imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}
	else
	{
		m_textureID = ImGui_ImplVulkan_AddTexture(sampler.sampler, m_defaultNonSupportedFileExtImage.m_imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
	}

	m_nameWithExt = m_name + "." + m_extension;
	m_displayName = m_nameWithExt;
}

std::string FileAsset::GetName()
{
	return m_name;
}

std::string FileAsset::GetPath()
{
	return m_path;
}

std::string FileAsset::GetExtension()
{
	return m_extension;
}

//VkImageView& FileAsset::GetThumbnail()
//{
//	return m_thumbnail;
//}

std::string FileAsset::GetNameWithExt()
{
	return m_nameWithExt;
}

std::string FileAsset::GetDisplayName()
{
	return m_displayName;
}

void FileAsset::SetDisplayName(std::string displayName)
{
	m_displayName = displayName;
}

ImTextureID& FileAsset::GetTextureID()
{
	return m_textureID;
}

void FileAsset::DestroyImage()
{
	m_thumbnailImage.DestroyImage();
}

void FileAsset::DestroyDefaultImage()
{
	m_defaultNonSupportedFileExtImage.DestroyImage();
}

bool FileAsset::IsOnDefaultTexture()
{
	return m_bIsOnDefaultTexture;
}
