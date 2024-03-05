#pragma once

#include "Defines.h"
#include "UI/UIWindow.h"
#include "UI/UIWidgets.h"
#include "UI/UIWindows/UIWindowMainToolBar.h"

//enum class TextureName
//{
//	PLAY_BUTTON = 0,
//	PAUSE_BUTTON,
//	STOP_BUTTON,
//};

//enum class EngineState
//{
//	PLAY = 0,
//	PAUSE,
//	ENGINE
//};

class UIWindowViewportScene : public UIWindow
{
public:

	libgfx_API UIWindowViewportScene();
	//-------------------------------------------------------------------Other Functions--------------------------------------------------------------------
	//This function is called every frame to update the UI of this window
	void libgfx_API Update(ImTextureID text) override;

	//----------------------------------------------------------------This Window Related Functions------------------------------------------------------------------

	//TODO: Place this in parent class
	//This function will be executed if m_bDoOnce is = true, the idea is to use this function only once at the beginning of Update() to initialize stuff in this window only
	void DoOnceForThisWindow();


	void libgfx_API SetArrayWidgetsInGame(std::vector<std::unique_ptr<UIWidgets>>& widgets);
	void libgfx_API UpdateArrayWidgetsInGameDatas(std::vector<std::unique_ptr<UIWidgets>>& widgets);
	void libgfx_API ClearWidgetsInGame();
	void libgfx_API ClearWidgetsImage();


	void libgfx_API ShowUIInGame(bool tf);
	bool libgfx_API IsUIShowing();

	void libgfx_API SetAnchorPositions(std::vector<std::unique_ptr<UIWidgets>>& widgets);
	float timer = 0.f;
	bool m_bShowTimer = true;
private: 
	//TODO: Place this in parent class
	//This boolean is used to call DoOnceForThisWindow() only once, it's initialized at true at the launching of the app and should never be reset to true during runtime
	bool m_bDoOnce = true;

	ImVec2 m_viewportScenePositionTopLeft;
	ImVec2 m_viewportScenePositionTopRight;
	ImVec2 m_viewportScenePositionBottomLeft;
	ImVec2 m_viewportScenePositionBottomRight;


	std::vector<UIWidgets> m_WidgetsInGame2;
	std::vector<std::unique_ptr<UIWidgets>> m_WidgetsInGame;

	bool m_bShowUIInGame = true;

	//ImVec2 m_anchorPosition;

	
	
};