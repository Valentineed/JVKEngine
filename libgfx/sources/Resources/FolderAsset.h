#pragma once

#include <string>
#include <vector>

#include "FileAsset.h"

class FolderAsset
{
public:
	FolderAsset() = default;
	FolderAsset(std::string path, std::string parentPath, std::string projectPath, int ID, Sampler& sampler);
	//FolderAsset(std::string path, FolderAsset* parent = nullptr);
	std::string GetName();
	void AddFile(std::string path, std::string projectPath, Sampler& sampler);
	std::string GetPath();
	std::string GetParentPath();

	std::vector<FileAsset>& GetAssetFiles();

	ImTextureID& GetTextureID();

	//VkImageView& GetThumbnail();
	int GetFolderID();

	void SetDisplayName(std::string name);

	std::string GetDisplayName();

	void DestroyImage();

private:
	std::string m_name;
	std::vector<FileAsset> m_assetFiles;
	std::string m_path;
	std::string m_parentPath;

	std::string m_displayName;

	int m_IDFolder;

	DataTexture m_thumbnailData;

	//VkImageView m_thumbnail;

	Image m_thumbnailImage;

	ImTextureID m_textureID;
};
