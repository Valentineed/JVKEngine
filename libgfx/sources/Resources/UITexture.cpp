#include "UITexture.h"
#include "TextureSampler.h"

#include "imgui_impl_vulkan.h"

UITexture::UITexture(Image image, Sampler& sampler) : m_image(image)
{
	m_textureID = ImGui_ImplVulkan_AddTexture(sampler.sampler, m_image.m_imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

void libgfx_API UITexture::ChangeImage(Image image, Sampler& sampler)
{
	m_image.DestroyImage();
	m_image = image;
	m_textureID = ImGui_ImplVulkan_AddTexture(sampler.sampler, m_image.m_imageView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
}

