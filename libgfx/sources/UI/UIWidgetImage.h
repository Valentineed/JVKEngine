#pragma once

#include "UIWidgets.h"
class UIWidgetImage : public UIWidgets
{
public:
	UIWidgetImage(Sampler& sampler, ImVec2 size = ImVec2(50, 50), ImVec2 pos = ImVec2(0, 0));
	UIWidgetImage(Sampler& sampler, ImVec2 size, ImVec2 pos, const char* path, Anchors anchor, ImVec2 anchorPosition, ImVec2 alignment);
	~UIWidgetImage() = default;
	libgfx_API ImVec2& GetSize();
	libgfx_API void SetSize(ImVec2& size);
	libgfx_API std::string& GetImagePath();
	void AddImageToWidget(Sampler& sampler, std::string path);
	ImTextureID& GetTextureID();
	void SetTextureID(ImTextureID& text);
	Image& GetDefaultImage();
	Image& GetImage();
	bool IsOnDefaultImage();

	std::vector<Image>& GetOldImagesToDestroy();



	ImVec2 libgfx_API CalculateFinalPosition();
	
private:
	ImVec2 m_size;
	Image m_defaultImage;
	Image m_image;
	std::vector<Image> m_oldImages;
	ImTextureID m_imageID;

	std::string m_imagePath = "";

	bool m_bIsOnDefautImage = true;


};

