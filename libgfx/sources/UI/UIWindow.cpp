#include "UIWindow.h"

#include "imgui.h"
#include <iostream>

void libgfx_API UIWindow::Update()
{
	if (m_bDoOnceForEveryWindow == true)
	{
		m_bDoOnceForEveryWindow = false;
		m_projectPath = std::filesystem::current_path();

		m_projectPath = m_projectPath;
		
		
		m_assetPath = m_projectPath;
		m_assetPath /= "Assets";

		
		m_resourcesPath = m_projectPath;
		m_resourcesPath /= "resources";
	}
}

#pragma warning(push, 0) // Removing all vulkan enum warnings because we're compiling in w4
libgfx_API void UIWindow::Update(ImTextureID text)
{
	
}

#pragma warning(pop)
void UIWindow::Show(bool tf)
{
	m_bShow = tf;
}

std::string& UIWindow::GetWindowName()
{
	return m_windowName;
}

void UIWindow::SetWindowMovable(bool tf)
{
	m_bIsWindowMovable = tf;
}

bool UIWindow::IsWindowMovable() const
{
	return m_bIsWindowMovable;
}

void libgfx_API UIWindow::AddFileToCurrentFolder()
{
}

bool UIWindow::IsSamplerCreated()
{
	return m_bIsTextureSamplerCreated;
}

bool UIWindow::IsTextureVectorEmpty()
{
	return m_textures.empty();
}

std::vector<UITexture>& UIWindow::GetTextures()
{
	return m_textures;
}

void UIWindow::CreateSampler()
{
	m_textureSampler = Sampler::CreateSampler(Sampler::Linear);
	m_bIsTextureSamplerCreated = true;
}

void UIWindow::SetWindowSpacePos()
{
	m_posTopLeftCorner = ImGui::GetWindowContentRegionMin();
	m_posBottomRightCorner = ImGui::GetWindowContentRegionMax();

	m_posTopLeftCorner.x += ImGui::GetWindowPos().x;
	m_posTopLeftCorner.y += ImGui::GetWindowPos().y;
	m_posBottomRightCorner.x += ImGui::GetWindowPos().x;
	m_posBottomRightCorner.y += ImGui::GetWindowPos().y;

	m_contentSize = ImVec2(m_posBottomRightCorner.x - m_posTopLeftCorner.x, m_posBottomRightCorner.y - m_posTopLeftCorner.y);
}

ImVec2& UIWindow::GetWindowSpaceTopLeftPos()
{
	return m_posTopLeftCorner;
}

ImVec2& UIWindow::GetWindowSpaceBottomRightPos()
{
	return m_posBottomRightCorner;
}

bool libgfx_API UIWindow::Begin(std::string name, ImGuiWindowFlags flags)
{
	return ImGui::Begin(name.c_str(), &m_bShow, flags);
}

void UIWindow::End()
{
	ImGui::End();
}