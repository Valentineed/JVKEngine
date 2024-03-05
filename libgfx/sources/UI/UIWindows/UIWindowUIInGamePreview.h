#pragma once

#include "UI/UIWindow.h"
#include "sol/sol.hpp"
#include "UI/UIWidgets.h"

enum class UIPreviewWindowTextureName
{
	BLACK_BACKGROUND = 0
};


class UIWindowUIInGamePreview : public UIWindow
{
public:

	libgfx_API UIWindowUIInGamePreview();
	//-------------------------------------------------------------------Other Functions--------------------------------------------------------------------
	//This function is called every frame to update the UI of this window
	void libgfx_API Update() override;



	//----------------------------------------------------------------This Window Related Functions------------------------------------------------------------------


	//TODO: Place this in parent class
	//This function will be executed if m_bDoOnce is = true, the idea is to use this function only once at the beginning of Update() to initialize stuff in this window only
	void DoOnceForThisWindow();

	void libgfx_API ClearWidgetsImage();
	void libgfx_API ClearWidgets();

	libgfx_API std::vector<std::unique_ptr<UIWidgets>>& GetWidgets();
	libgfx_API Sampler& GetSampler();
	libgfx_API bool NeedReloadViewportWidgets();
	void libgfx_API SetNeedReloadViewportWidgets(bool tf);

	libgfx_API bool NeedReloadViewportWidgetsDatas();
	void libgfx_API SetNeedReloadViewportWidgetsDatas(bool tf);
	libgfx_API void AddWidgetImage(const char* path, Anchors anchor, ImVec2 anchorPosition, ImVec2 alignment, ImVec2 size = ImVec2(50, 50), ImVec2 pos = ImVec2(0, 0));
	libgfx_API void AddWidgetText(const char* text, Anchors anchor, ImVec2 anchorPosition, ImVec2 alignment, ImVec2 pos = ImVec2(0, 0));

	libgfx_API bool GetShowUIInVPWindow();
	libgfx_API void SetShowUIInVPWindow(bool tf);


	libgfx_API std::vector<std::string>& GetAnchorsAvailable();
	libgfx_API int GetCurrentAnchorIndex();

	libgfx_API void SetCurrentAnchorIndex(int index);

	libgfx_API bool NeedReloadAnchors();
	libgfx_API void SetNeedReloadAnchors(bool tf);

	std::vector<Image>& GetOldImagesToDestroy();
	
private:
	//TODO: Place this in parent class
	//This boolean is used to call DoOnceForThisWindow() only once, it's initialized at true at the launching of the app and should never be reset to true during runtime
	bool m_bDoOnce = true;

	//sol::state m_lua;

	//sol::table m_widgetsData;
	std::vector<std::string> m_widgetsTypeAvailable;
	int m_currentChosenWidgetType = 0;
	std::vector<UIWidgets*> m_widgets2;
	std::vector<std::unique_ptr<UIWidgets>> m_widgets;
	std::vector<Image> m_oldImagesToDestroy;
	bool m_bReloadViewportWidgets = false;
	bool m_bReloadViewportWidgetsDatas = false;

	bool m_bShowUIInViewportWindow = true;
	bool m_bReloadAnchors = false;

	std::vector<std::string> m_AnchorsAvailable;
	int m_currentAnchorSelected = 0;
};