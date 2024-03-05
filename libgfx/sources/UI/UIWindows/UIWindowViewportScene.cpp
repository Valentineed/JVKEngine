#include "UIWindowViewportScene.h"
#include "Image.h"
#include "UI/UIWidgets.h"
#include "Resources/TextureLoader.h"

#include <imgui_internal.h>

#include "UI/UIWidgetImage.h"
#include "UI/UIWidgetText.h"

UIWindowViewportScene::UIWindowViewportScene()
{
	m_windowName = "ViewportSceneWindow";

	
}

void libgfx_API UIWindowViewportScene::Update(ImTextureID text)
{
	(void)text;
	UIWindow::Update();

#ifndef _SHIPPING
	DoOnceForThisWindow();
#endif

	if (m_bIsWindowMovable == true)
	{
		m_windowFlags = ImGuiWindowFlags_None;
	}
	else
	{
		m_windowFlags = ImGuiWindowFlags_NoMove;
	}

#ifdef _SHIPPING
	m_windowFlags |= ImGuiWindowFlags_NoTitleBar;
#endif

	//TODO: Change hard coding values later
	if (!Begin("ViewportSceneWindow", m_windowFlags))
	{
		End();
	}
	else
	{
		SetWindowSpacePos();

		m_viewportScenePositionTopLeft = ImGui::GetCursorPos();
		m_viewportScenePositionTopRight = ImVec2(ImGui::GetCursorPosX() + m_contentSize.x, ImGui::GetCursorPosY());
		m_viewportScenePositionBottomLeft = ImVec2(ImGui::GetCursorPosX(), ImGui::GetCursorPosY() + m_contentSize.y);
		m_viewportScenePositionBottomRight = ImVec2(ImGui::GetCursorPosX() + m_contentSize.x, ImGui::GetCursorPosY() + m_contentSize.y);

		SetAnchorPositions(m_WidgetsInGame);
		
#ifndef _SHIPPING
		
		//auto BasePosition = ImGui::GetCursorPos();
		//ImGui::Image(text, ImVec2(1400.f, 647.f));
		ImGui::Image(text, ImVec2(m_contentSize.x, m_contentSize.y));

#else
		auto BasePosition = ImGui::GetCursorPos();
		//ImGui::Image(text, ImVec2(1902, 969));
		ImGui::Image(text, ImVec2(m_contentSize.x, m_contentSize.y));
#endif
		if (m_bShowUIInGame == true)
		{
			if (m_bShowTimer == true)
			{
				ImGui::SetCursorPos(m_viewportScenePositionTopLeft);

				ImGui::Text("Timer :");
				ImGui::SameLine();
				ImGui::Text(std::to_string(timer).c_str());
			}
			
			
			if (m_WidgetsInGame.empty() == false)
			{
				for (auto& widget : m_WidgetsInGame)
				{
					if (widget->GetType() == WidgetsType::IMAGE && widget->IsVisible() == true)
					{
						ImGui::SetCursorPos(static_cast<UIWidgetImage*>(widget.get())->CalculateFinalPosition());
						ImGui::Image(static_cast<UIWidgetImage*>(widget.get())->GetTextureID(), static_cast<UIWidgetImage*>(widget.get())->GetSize());
					}
					else if(widget->GetType() == WidgetsType::TEXT && widget->IsVisible() == true)
					{
						ImGui::SetCursorPos(static_cast<UIWidgetText*>(widget.get())->CalculateFinalPosition());
						ImGui::Text(static_cast<UIWidgetText*>(widget.get())->m_bufferText);
					}

					
				}
			}
		}
		

		End();
	}
}

void UIWindowViewportScene::DoOnceForThisWindow()
{
	if (m_bDoOnce == true)
	{
		//CreateSampler();

		m_bDoOnce = false;
	}
}

void libgfx_API UIWindowViewportScene::SetArrayWidgetsInGame(std::vector<std::unique_ptr<UIWidgets>>& widgets)
{
	for (auto& widget : m_WidgetsInGame)
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
	m_WidgetsInGame.clear();
	
	for (auto& widget : widgets)
	{
		if (widget.get()->GetType() == WidgetsType::IMAGE)
		{
			m_WidgetsInGame.push_back(std::make_unique<UIWidgetImage>(m_textureSampler,
				static_cast<UIWidgetImage*>(widget.get())->GetSize(),
				widget->GetPos(),
				static_cast<UIWidgetImage*>(widget.get())->GetImagePath().c_str(),
				widget->GetAnchor(),
				widget->GetAnchorPosition(),
				widget->GetAlignment()));
		}
		else if (widget.get()->GetType() == WidgetsType::TEXT)
		{
			m_WidgetsInGame.push_back(std::make_unique<UIWidgetText>(widget->GetPos()));
		}
	}
}

void libgfx_API UIWindowViewportScene::UpdateArrayWidgetsInGameDatas(std::vector<std::unique_ptr<UIWidgets>>& widgets)
{
	
	for (int index = 0; index < m_WidgetsInGame.size(); index++)
	{
		m_WidgetsInGame[index].get()->SetPos(widgets[index].get()->GetPos());
		m_WidgetsInGame[index].get()->SetIsVisible(widgets[index].get()->IsVisible());
		m_WidgetsInGame[index].get()->SetAlignment(widgets[index].get()->GetAlignment());
		
		if (widgets[index].get()->GetType() == WidgetsType::IMAGE)
		{
			static_cast<UIWidgetImage*>(m_WidgetsInGame[index].get())->SetSize(static_cast<UIWidgetImage*>(widgets[index].get())->GetSize());
			static_cast<UIWidgetImage*>(m_WidgetsInGame[index].get())->SetTextureID(static_cast<UIWidgetImage*>(widgets[index].get())->GetTextureID());
		}
		else if (widgets[index].get()->GetType() == WidgetsType::TEXT)
		{
			strncpy(static_cast<UIWidgetText*>(m_WidgetsInGame[index].get())->m_bufferText, static_cast<UIWidgetText*>(widgets[index].get())->m_bufferText, 1000);
		}
	}
}

void libgfx_API UIWindowViewportScene::ClearWidgetsInGame()
{
	m_WidgetsInGame.clear();
}

void libgfx_API UIWindowViewportScene::ClearWidgetsImage()
{
	for each (auto & widget in m_WidgetsInGame)
	{
		if (widget->GetType() == WidgetsType::IMAGE)
		{
			static_cast<UIWidgetImage*>(widget.get())->GetDefaultImage().DestroyImage();
		}
		if (static_cast<UIWidgetImage*>(widget.get())->IsOnDefaultImage() == false)
		{
			static_cast<UIWidgetImage*>(widget.get())->GetImage().DestroyImage();
		}
	}
}

void UIWindowViewportScene::ShowUIInGame(bool tf)
{
	m_bShowUIInGame = tf;
}

bool libgfx_API UIWindowViewportScene::IsUIShowing()
{
	return m_bShowUIInGame;
}

void libgfx_API UIWindowViewportScene::SetAnchorPositions(std::vector<std::unique_ptr<UIWidgets>>& widgets)
{
	for (int index = 0; index < m_WidgetsInGame.size(); index++)
	{
		m_WidgetsInGame[index].get()->SetAnchorVP(widgets[index].get()->GetAnchor(),
			m_viewportScenePositionTopLeft, 
			m_viewportScenePositionTopRight,
			m_viewportScenePositionBottomLeft,
			m_viewportScenePositionBottomRight);
		
	}
}

//void UIWindowViewportScene::SetAnchorPosition(Anchors anchor)
//{
//	switch (anchor)
//	{
//		case Anchors::TOP_LEFT:
//			m_anchorPosition = ImGui::GetWindowContentRegionMin();
//			break;
//		case Anchors::TOP_MIDDLE:
//			m_anchorPosition = ImVec2(m_contentSize.x / 2, m_posTopLeftCorner.y);
//			break;
//		case Anchors::TOP_RIGHT:
//			m_anchorPosition = ImVec2(m_posBottomRightCorner.x, m_posTopLeftCorner.y);
//			break;
//		case Anchors::MIDDLE_LEFT:
//			m_anchorPosition = ImVec2(m_posTopLeftCorner.x, m_contentSize.y / 2);
//			break;
//		case Anchors::MIDDLE:
//			m_anchorPosition = ImVec2(m_contentSize.x / 2, m_contentSize.y / 2);
//			break;
//		case Anchors::MIDDLE_RIGHT:
//			m_anchorPosition = ImVec2(m_posBottomRightCorner.x, m_contentSize.y / 2);
//			break;
//		case Anchors::BOTTOM_LEFT:
//			m_anchorPosition = ImVec2(m_posTopLeftCorner.x, m_posBottomRightCorner.y);
//			break;
//		case Anchors::BOTTOM_MIDDLE:
//			m_anchorPosition = ImVec2(m_contentSize.x / 2, m_posBottomRightCorner.y);
//			break;
//		case Anchors::BOTTOM_RIGHT:
//			m_anchorPosition = m_posBottomRightCorner;
//			break;
//	}
//}
