#include "UIWindowMainToolBar.h"

#include <imgui_internal.h>

#include "Resources/TextureLoader.h"
#include "UI/UIWidgets.h"

UIWindowMainToolBar::UIWindowMainToolBar()
{
	m_windowName = "MainToolBarWindow";
}

void libgfx_API UIWindowMainToolBar::Update()
{
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

	//TODO: Change hard coding values later
	if (!Begin("MainToolBarWindow", m_windowFlags))
	{
		End();
	}
	else
	{
		SetWindowSpacePos();

		if (ImGui::Button("Save scene"))
		{
			m_saveSceneDelegate();
		}
		ImGui::SameLine();
		ImGui::Dummy(ImVec2((ImGui::GetWindowWidth() / 2) - 100.f, 0.0f));
		ImGui::SameLine();
		IUIWidgets::CreateViewportButton("##StopButton", m_bIsStopButtonClicked, m_stopButtonColor, m_textures[static_cast<int>(TextureName::STOP_BUTTON)].GetTextureID(), this, &UIWindowMainToolBar::LogicViewportButton);
		ImGui::SameLine();

		if (m_bIsPlayButtonClicked == false)
		{
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			IUIWidgets::CreateViewportButton("##PauseButton", m_bIsPauseButtonClicked, m_pauseButtonColor, m_textures[static_cast<int>(TextureName::PAUSE_BUTTON)].GetTextureID(), this, &UIWindowMainToolBar::LogicViewportButton);
			ImGui::PopItemFlag();
		}
		else
		{
			IUIWidgets::CreateViewportButton("##PauseButton", m_bIsPauseButtonClicked, m_pauseButtonColor, m_textures[static_cast<int>(TextureName::PAUSE_BUTTON)].GetTextureID(), this, &UIWindowMainToolBar::LogicViewportButton);
		}

		ImGui::SameLine();
		IUIWidgets::CreateViewportButton("##PlayButton", m_bIsPlayButtonClicked, m_playButtonColor, m_textures[static_cast<int>(TextureName::PLAY_BUTTON)].GetTextureID(), this, &UIWindowMainToolBar::LogicViewportButton);




		End();
	}
}

void UIWindowMainToolBar::DoOnceForThisWindow()
{
	if (m_bDoOnce == true)
	{
		CreateSampler();

		std::filesystem::path PlayButtonTexturePath = m_resourcesPath;

		PlayButtonTexturePath /= "UI/Buttons/PlayButton.png";

		std::filesystem::path PauseButtonTexturePath = m_resourcesPath;

		PauseButtonTexturePath /= "UI/Buttons/PauseButton.png";

		std::filesystem::path StopButtonTexturePath = m_resourcesPath;

		StopButtonTexturePath /= "UI/Buttons/StopButton.png";

		m_textures.emplace_back(DataTexture::CreateTexture(PlayButtonTexturePath.string(), DataTexture::RGBA, false), m_textureSampler);
		m_textures.emplace_back(DataTexture::CreateTexture(PauseButtonTexturePath.string(), DataTexture::RGBA, false), m_textureSampler);
		m_textures.emplace_back(DataTexture::CreateTexture(StopButtonTexturePath.string(), DataTexture::RGBA, false), m_textureSampler);

		m_bDoOnce = false;
	}
}

void UIWindowMainToolBar::LogicViewportButton(bool& isButtonPressed, ImVec4& ButtonColor)
{
	m_bIsChangingState = true;
	isButtonPressed = !isButtonPressed;

	if (m_bIsStopButtonClicked == true)
	{
		m_bIsPlayButtonClicked = false;
		m_playButtonColor = UI_VIEWPORT_BUTTON_STATE_COLOR_UNPRESSED;
		m_bIsPauseButtonClicked = false;
		m_pauseButtonColor = UI_VIEWPORT_BUTTON_STATE_COLOR_UNPRESSED;
		m_bIsStopButtonClicked = false;

		m_EngineState = EngineState::ENGINE;
	}
	else
	{
		if (isButtonPressed == false)
		{
			ButtonColor = UI_VIEWPORT_BUTTON_STATE_COLOR_UNPRESSED;
		}
		else
		{
			ButtonColor = UI_VIEWPORT_BUTTON_STATE_COLOR_PRESSED;
		}

		if (m_bIsPlayButtonClicked == true && m_bIsPauseButtonClicked == false)
		{
			m_EngineState = EngineState::PLAY;
		}
		else if (m_bIsPlayButtonClicked == true && m_bIsPauseButtonClicked == true)
		{
			m_EngineState = EngineState::PAUSE;
		}
		else if (m_bIsPlayButtonClicked == false && m_bIsPauseButtonClicked == true)
		{
			m_bIsPauseButtonClicked = false;
			m_pauseButtonColor = UI_VIEWPORT_BUTTON_STATE_COLOR_UNPRESSED;
			m_EngineState = EngineState::ENGINE;
		}
		else
		{
			m_EngineState = EngineState::ENGINE;
		}
	}
}

EngineState libgfx_API UIWindowMainToolBar::GetEngineState()
{
	return m_EngineState;
}

void libgfx_API UIWindowMainToolBar::SetEngineState(EngineState state)
{
	m_EngineState = state;
}

bool libgfx_API UIWindowMainToolBar::IsChangingState()
{
	return m_bIsChangingState;
}

void libgfx_API UIWindowMainToolBar::SetChangingState(bool tf)
{
	m_bIsChangingState = tf;
}

void libgfx_API UIWindowMainToolBar::ResetToEngineState()
{
	m_bIsChangingState = true;

	m_bIsPlayButtonClicked = false;
	m_playButtonColor = UI_VIEWPORT_BUTTON_STATE_COLOR_UNPRESSED;
	m_bIsPauseButtonClicked = false;
	m_pauseButtonColor = UI_VIEWPORT_BUTTON_STATE_COLOR_UNPRESSED;

	m_EngineState = EngineState::ENGINE;
}

libgfx_API Delegate<void()>& UIWindowMainToolBar::GetSaveSceneDelegate()
{
	return m_saveSceneDelegate;
}
