#pragma once

#include "Image.h"

#include <imgui_impl_glfw.h>

class UITexture
{
public:
	UITexture(Image image, Sampler& sampler);

	void libgfx_API ChangeImage(Image image, Sampler& sampler);
	ImTextureID& GetTextureID() { return m_textureID; }
	Image& GetImage() { return m_image; }
private:
	Image m_image;
	ImTextureID m_textureID;
};
