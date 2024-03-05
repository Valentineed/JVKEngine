#include "FolderAsset.h"
#include "imgui_impl_vulkan.h"

#include <filesystem>

FolderAsset::FolderAsset(std::string path, std::string parentPath, std::string projectPath, int ID, Sampler& sampler) : m_path(path), m_parentPath(parentPath), m_IDFolder(ID)
{
	std::size_t indexLastSlash = path.find_last_of('\\');
	m_name = path.substr(indexLastSlash + 1);

	std::filesystem::path folderTexturePath = projectPath;
	folderTexturePath /= "UI/Icons";
	folderTexturePath /= "folder.png";

	//TODO: Avoid creating a sampler for each file (cause the generic one that is used in graphidcApi disapeared)
	//m_sampler = Sampler::CreateSampler(Sampler::Linear);
	
	m_thumbnailImage = m_thumbnailData.CreateTexture(folderTexturePath.string(), DataTexture::RGBA, false);


	m_textureID = ImGui_ImplVulkan_AddTexture(sampler.sampler, m_thumbnailImage.m_imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	m_displayName = m_name;
}

void FolderAsset::AddFile(std::string path, std::string projectPath, Sampler& sampler)
{
	m_assetFiles.emplace_back(path, projectPath, sampler);
}

std::vector<FileAsset>& FolderAsset::GetAssetFiles()
{
	return m_assetFiles;
}

ImTextureID& FolderAsset::GetTextureID()
{
	return m_textureID;
}

std::string FolderAsset::GetName()
{
	return m_name;
}
//
//void FolderAsset::AddFile(std::string path, std::string projectPath)
//{
//	m_assetFiles.emplace_back(path, projectPath);
//}
//
std::string FolderAsset::GetPath()
{
	return m_path;
}

std::string FolderAsset::GetParentPath()
{
	return m_parentPath;
}

//VkImageView& FolderAsset::GetThumbnail()
//{
//	/*return m_thumbnail;*/
//	
//}

int FolderAsset::GetFolderID()
{
	return m_IDFolder;
}

void FolderAsset::SetDisplayName(std::string displayName)
{
	m_displayName = displayName;
}
//
std::string FolderAsset::GetDisplayName()
{
	return m_displayName;
}

void FolderAsset::DestroyImage()
{
	m_thumbnailImage.DestroyImage();
}

