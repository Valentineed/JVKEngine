#include "UIWindowUIInGamePreview.h"
#include "UI/UIWidgets.h"
#include "Resources/TextureLoader.h"

#include <windows.h>
#include <commdlg.h>

#include "ToolsEngine.h"
#include "UI/UIWidgetImage.h"
#include "UI/UIWidgetText.h"

UIWindowUIInGamePreview::UIWindowUIInGamePreview()
{
	m_bShow = true;
	m_windowName = "UIPreviewWindow";

	m_widgetsTypeAvailable.emplace_back("Text");
	m_widgetsTypeAvailable.emplace_back("Image");

	m_AnchorsAvailable.emplace_back("Top Left");
	m_AnchorsAvailable.emplace_back("Top Middle");
	m_AnchorsAvailable.emplace_back("Top Right");
	m_AnchorsAvailable.emplace_back("Middle Left");
	m_AnchorsAvailable.emplace_back("Middle");
	m_AnchorsAvailable.emplace_back("Middle Right");
	m_AnchorsAvailable.emplace_back("Bottom Left");
	m_AnchorsAvailable.emplace_back("Bottom Middle");
	m_AnchorsAvailable.emplace_back("Bottom Right");

	
}

void libgfx_API UIWindowUIInGamePreview::Update()
{
	if (m_bShow)
	{
		UIWindow::Update();

		DoOnceForThisWindow();

		if (m_bIsWindowMovable == true)
		{
			m_windowFlags = ImGuiWindowFlags_None | ImGuiWindowFlags_NoResize;
		}
		else
		{
			m_windowFlags = ImGuiWindowFlags_NoMove;
		}

		ImGui::SetNextWindowSize(ImVec2(1500.f, 730.f));
		if (!Begin("UIPreviewWindow", m_windowFlags))
		{
			End();
		}
		else
		{
			
		if (ImGui::Button("Add Widget"))
		{
			ImGui::OpenPopup("Popup Add widget");
		}

		if (ImGui::Checkbox("Show UI in Editor", &m_bShowUIInViewportWindow))
		{
			m_bReloadViewportWidgetsDatas = true;
		}

		if (ImGui::BeginPopup("Popup Add widget"))
		{
			if (ImGui::BeginCombo("Widgets Type", m_widgetsTypeAvailable[m_currentChosenWidgetType].c_str()))
			{
				for (int n = 0; n < m_widgetsTypeAvailable.size(); n++)
				{
					const bool is_selected = (m_currentChosenWidgetType == n);
					if (ImGui::Selectable(m_widgetsTypeAvailable[n].c_str(), is_selected))
						m_currentChosenWidgetType = n;

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

			if (ImGui::Button("Create new widget"))
			{
					//m_widgets.emplace_back(static_cast<WidgetsType>(m_currentChosenWidgetType), m_textureSampler);
				if (m_currentChosenWidgetType == (int)WidgetsType::IMAGE)
				{
					m_widgets.push_back(std::make_unique<UIWidgetImage>(m_textureSampler));
					m_bReloadViewportWidgets = true;
				}
				else if (m_currentChosenWidgetType == (int)WidgetsType::TEXT)
				{
					m_widgets.push_back(std::make_unique<UIWidgetText>());
					m_bReloadViewportWidgets = true;
				}
				
				
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}

		ImGui::Separator();

		for (int i = 0; i < m_widgets.size(); i++)
		{
			std::string widName = "Widget " + std::to_string(i);
			
			if (ImGui::TreeNode(widName.c_str()))
			{
				if (ImGui::Button("Remove widget"))
				{
					if (m_widgets[i]->GetType() == WidgetsType::IMAGE)
					{
						m_oldImagesToDestroy.push_back(std::move(static_cast<UIWidgetImage*>(m_widgets[i].get())->GetDefaultImage()));
						if (static_cast<UIWidgetImage*>(m_widgets[i].get())->IsOnDefaultImage() == false)
						{
							m_oldImagesToDestroy.push_back(std::move(static_cast<UIWidgetImage*>(m_widgets[i].get())->GetImage()));
						}
					}
					
					
					m_widgets.erase(m_widgets.begin() + i);
	
					m_bReloadViewportWidgets = true;
					ImGui::TreePop();

					End();
					return;
				}
				
				if (ImGui::BeginCombo("Anchors", m_AnchorsAvailable[(int)m_widgets[i]->GetAnchor()].c_str()))
				{
					for (int n = 0; n < m_AnchorsAvailable.size(); n++)
					{
						const bool is_selected = ((int)m_widgets[i]->GetAnchor() == n);
						if (ImGui::Selectable(m_AnchorsAvailable[n].c_str(), is_selected))
						{
							m_widgets[i]->SetAnchor((Anchors)n);
							m_bReloadAnchors = true;
						}
							

						// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
						if (is_selected)
							ImGui::SetItemDefaultFocus();
					}
					ImGui::EndCombo();
				}

				if (ImGui::DragFloat("Alignment X", &m_widgets[i]->GetAlignment().x, 0.01f, 0.f, 1.f))
				{
					m_bReloadViewportWidgetsDatas = true;
				}


				if (ImGui::DragFloat("Alignment Y", &m_widgets[i]->GetAlignment().y, 0.01f, 0.f, 1.f))
				{
					m_bReloadViewportWidgetsDatas = true;
				}
				
				if (ImGui::Checkbox("Is Visible", &m_widgets[i]->IsVisible()))
				{
					m_bReloadViewportWidgetsDatas = true;
				}
				
				if (ImGui::DragFloat("PosX", &m_widgets[i]->GetPos().x, 1.f))
				{
					m_bReloadViewportWidgetsDatas = true;
				}
				if (ImGui::DragFloat("PosY", &m_widgets[i]->GetPos().y, 1.f))
				{
					m_bReloadViewportWidgetsDatas = true;
				}

				if (m_widgets[i]->GetType() == WidgetsType::IMAGE)
				{
					if (ImGui::DragFloat("SizeX", &static_cast<UIWidgetImage*>(m_widgets[i].get())->GetSize().x, 1.f))
					{
						m_bReloadViewportWidgetsDatas = true;
					}
					if (ImGui::DragFloat("SizeY", &static_cast<UIWidgetImage*>(m_widgets[i].get())->GetSize().y, 1.f))
					{
						m_bReloadViewportWidgetsDatas = true;
					}

					std::string SelectableImageName = "##Image" + widName;
					auto OldImagePos = ImGui::GetCursorPos();
					if (ImGui::Selectable(SelectableImageName.c_str(), false, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(100, 100)))
					{
						if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
						{
							std::string PathFound = ToolsEngine::OpenFileName();

							if (PathFound.empty() == false)
							{
								static_cast<UIWidgetImage*>(m_widgets[i].get())->AddImageToWidget(m_textureSampler, PathFound);
								m_bReloadViewportWidgetsDatas = true;
							}
						}
					}

					ImGui::SetCursorPos(OldImagePos);
					ImGui::Image(static_cast<UIWidgetImage*>(m_widgets[i].get())->GetTextureID(), ImVec2(100, 100));
				}

				if (m_widgets[i]->GetType() == WidgetsType::TEXT)
				{
						//ImGui::SetCursorPos(ImVec2(m_widgets[i].GetPos().x, m_widgets[i].GetPos().y));
					if (ImGui::InputText("Text", static_cast<UIWidgetText*>(m_widgets[i].get())->m_bufferText, 2048))
					{
						m_bReloadViewportWidgetsDatas = true;
					}
				}
				

				ImGui::TreePop();
			}
		}
			End();
		}
	}
}

void UIWindowUIInGamePreview::DoOnceForThisWindow()
{
	if (m_bDoOnce == true)
	{
		m_bIsWindowMovable = true;
		m_bDoOnce = false;

		//CreateSampler();

		std::filesystem::path BlackBackgroundTexturePath = m_resourcesPath;

		BlackBackgroundTexturePath /= "UI/Background/black.png";

		m_textures.emplace_back(DataTexture::CreateTexture(BlackBackgroundTexturePath.string(), DataTexture::RGBA, false), m_textureSampler);
	}
}

void libgfx_API UIWindowUIInGamePreview::ClearWidgetsImage()
{
	for each (auto& widget in m_widgets)
	{
		if (widget->GetType() == WidgetsType::IMAGE)
		{
			static_cast<UIWidgetImage*>(widget.get())->GetDefaultImage().DestroyImage();
		}
		if (static_cast<UIWidgetImage*>(widget.get())->IsOnDefaultImage() == false)
		{
			static_cast<UIWidgetImage*>(widget.get())->GetImage().DestroyImage();
		}

		for (auto& image : static_cast<UIWidgetImage*>(widget.get())->GetOldImagesToDestroy())
		{
			image.DestroyImage();
		}
	}

	for each (auto Image in m_oldImagesToDestroy)
	{
		Image.DestroyImage();
	}
}

void libgfx_API UIWindowUIInGamePreview::ClearWidgets()
{
	for (auto& widget : m_widgets)
	{
		if (widget->GetType() == WidgetsType::IMAGE)
		{
			static_cast<UIWidgetImage*>(widget.get())->GetDefaultImage().DestroyImage();
			if (static_cast<UIWidgetImage*>(widget.get())->IsOnDefaultImage() == false)
			{
				static_cast<UIWidgetImage*>(widget.get())->GetImage().DestroyImage();
			}
		}	
	}
	m_widgets.clear();
}

std::vector<std::unique_ptr<UIWidgets>>& UIWindowUIInGamePreview::GetWidgets()
{
	return m_widgets;
}

libgfx_API Sampler& UIWindowUIInGamePreview::GetSampler()
{
	return m_textureSampler;
}

libgfx_API bool UIWindowUIInGamePreview::NeedReloadViewportWidgets()
{
	return m_bReloadViewportWidgets;
}

void libgfx_API UIWindowUIInGamePreview::SetNeedReloadViewportWidgets(bool tf)
{
	m_bReloadViewportWidgets = tf;
}

libgfx_API bool UIWindowUIInGamePreview::NeedReloadViewportWidgetsDatas()
{
	return m_bReloadViewportWidgetsDatas;
}

void libgfx_API UIWindowUIInGamePreview::SetNeedReloadViewportWidgetsDatas(bool tf)
{
	m_bReloadViewportWidgetsDatas = tf;
}

libgfx_API void UIWindowUIInGamePreview::AddWidgetImage(const char* path, Anchors anchor, ImVec2 anchorPosition, ImVec2 alignment, ImVec2 size, ImVec2 pos)
{
	m_widgets.push_back(std::make_unique<UIWidgetImage>(m_textureSampler, size, pos, path, anchor, anchorPosition, alignment));
}

libgfx_API void UIWindowUIInGamePreview::AddWidgetText(const char* text, Anchors anchor, ImVec2 anchorPosition, ImVec2 alignment, ImVec2 pos)
{
	m_widgets.push_back(std::make_unique<UIWidgetText>(text, anchor, anchorPosition, alignment, pos));
}

libgfx_API bool UIWindowUIInGamePreview::GetShowUIInVPWindow()
{
	return m_bShowUIInViewportWindow;
}

libgfx_API void UIWindowUIInGamePreview::SetShowUIInVPWindow(bool tf)
{
	m_bShowUIInViewportWindow = tf;
}

libgfx_API std::vector<std::string>& UIWindowUIInGamePreview::GetAnchorsAvailable()
{
	return m_AnchorsAvailable;
}

libgfx_API int UIWindowUIInGamePreview::GetCurrentAnchorIndex()
{
	return m_currentAnchorSelected;
}

libgfx_API void UIWindowUIInGamePreview::SetCurrentAnchorIndex(int index)
{
	m_currentAnchorSelected = index;
}

libgfx_API bool UIWindowUIInGamePreview::NeedReloadAnchors()
{
	return m_bReloadAnchors;
}

libgfx_API void UIWindowUIInGamePreview::SetNeedReloadAnchors(bool tf)
{
	m_bReloadAnchors = tf;
}

std::vector<Image>& UIWindowUIInGamePreview::GetOldImagesToDestroy()
{
	return m_oldImagesToDestroy;
}
