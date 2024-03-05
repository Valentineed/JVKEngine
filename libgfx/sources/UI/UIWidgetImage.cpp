#include "UIWidgetImage.h"

#include "imgui_impl_vulkan.h"
#include "Resources/TextureLoader.h"

#include <filesystem>

UIWidgetImage::UIWidgetImage(Sampler& sampler, ImVec2 size, ImVec2 pos)
{
	auto DefaultTexture = std::filesystem::current_path().string() + "\\resources\\UI\\Background\\black.png";

	m_imagePath = DefaultTexture;
	m_defaultImage = DataTexture::CreateTexture(DefaultTexture, DataTexture::RGBA, false);

	m_imageID = ImGui_ImplVulkan_AddTexture(sampler.sampler, m_defaultImage.m_imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	m_typeString = "Image";
	m_type = WidgetsType::IMAGE;
	m_pos = pos;
	m_size = size;

	
}

UIWidgetImage::UIWidgetImage(Sampler& sampler, ImVec2 size, ImVec2 pos, const char* path, Anchors anchor, ImVec2 anchorPosition, ImVec2 alignment)
{
	m_imagePath = path;
	m_defaultImage = DataTexture::CreateTexture(path, DataTexture::RGBA, false);

	m_imageID = ImGui_ImplVulkan_AddTexture(sampler.sampler, m_defaultImage.m_imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

	m_typeString = "Image";
	m_type = WidgetsType::IMAGE;
	m_pos = pos;
	m_size = size;

	m_anchor = anchor;
	m_anchorPosition = anchorPosition;
	m_alignment = alignment;
	
}

libgfx_API ImVec2& UIWidgetImage::GetSize()
{
	return m_size;
}

libgfx_API void UIWidgetImage::SetSize(ImVec2& size)
{
	m_size = size;
}

libgfx_API std::string& UIWidgetImage::GetImagePath()
{
	return m_imagePath;
}

void UIWidgetImage::AddImageToWidget(Sampler& sampler, std::string path)
{
	if (m_type == WidgetsType::IMAGE)
	{
		m_imagePath = path;
		auto count = m_imagePath.find("Assets\\");
		if (count == std::string::npos)
		{
			count = m_imagePath.find("resources\\");
		}
		m_imagePath.erase(0, count);
		/*m_image.DestroyImage();*/

		/*if (m_bIsOnDefautImage == false)
		{
			m_oldImages.push_back(std::move(m_image));
		}*/
		
		m_image = DataTexture::CreateTexture(path, DataTexture::RGBA, false);
		
		m_imageID = ImGui_ImplVulkan_AddTexture(sampler.sampler, m_image.m_imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

		m_bIsOnDefautImage = false;
	}
}

ImTextureID& UIWidgetImage::GetTextureID()
{
	return m_imageID;
}

void UIWidgetImage::SetTextureID(ImTextureID& text)
{
	m_imageID = text;
}

Image& UIWidgetImage::GetDefaultImage()
{
	return m_defaultImage;
}

Image& UIWidgetImage::GetImage()
{
	return m_image;
}

bool UIWidgetImage::IsOnDefaultImage()
{
	return m_bIsOnDefautImage;
}

std::vector<Image>& UIWidgetImage::GetOldImagesToDestroy()
{
	return m_oldImages;
}

ImVec2 libgfx_API UIWidgetImage::CalculateFinalPosition()
{
	float finalX = (GetAnchorPosition().x + GetPos().x) - (m_alignment.x * m_size.x);
	float finalY = (GetAnchorPosition().y + GetPos().y) - (m_alignment.y * m_size.y);

	return ImVec2(finalX, finalY);
}
