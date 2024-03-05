#pragma once
#include <imgui_impl_glfw.h>
#include <libgfx_Export.h>


#include "Defines.h"
#include "UI/UIWindow.h"

enum class EngineState
{
	PLAY = 0,
	PAUSE,
	ENGINE
};

enum class TextureName
{
	PLAY_BUTTON = 0,
	PAUSE_BUTTON,
	STOP_BUTTON,
};


class UIWindowMainToolBar : public UIWindow
{
public:
	libgfx_API UIWindowMainToolBar();
	//-------------------------------------------------------------------Other Functions--------------------------------------------------------------------
	//This function is called every frame to update the UI of this window
	void libgfx_API Update() override;

	//----------------------------------------------------------------This Window Related Functions------------------------------------------------------------------

	//TODO: Place this in parent class
	//This function will be executed if m_bDoOnce is = true, the idea is to use this function only once at the beginning of Update() to initialize stuff in this window only
	void DoOnceForThisWindow();

	void LogicViewportButton(bool& isButtonPressed, ImVec4& ButtonColor);

	EngineState libgfx_API GetEngineState();

	void libgfx_API SetEngineState(EngineState state);

	bool libgfx_API IsChangingState();

	void libgfx_API SetChangingState(bool tf);

	void libgfx_API ResetToEngineState();

	libgfx_API Delegate<void()>& GetSaveSceneDelegate();
private:
	//TODO: Place this in parent class
	//This boolean is used to call DoOnceForThisWindow() only once, it's initialized at true at the launching of the app and should never be reset to true during runtime
	bool m_bDoOnce = true;

	bool m_bIsPlayButtonClicked = false;
	bool m_bIsPauseButtonClicked = false;
	bool m_bIsStopButtonClicked = false;


	ImVec4 m_playButtonColor = UI_VIEWPORT_BUTTON_STATE_COLOR_UNPRESSED;
	ImVec4 m_pauseButtonColor = UI_VIEWPORT_BUTTON_STATE_COLOR_UNPRESSED;
	ImVec4 m_stopButtonColor = UI_VIEWPORT_BUTTON_STATE_COLOR_UNPRESSED;

	EngineState m_EngineState = EngineState::ENGINE;

	bool m_bIsChangingState = false;

	Delegate<void()> m_saveSceneDelegate;
};

