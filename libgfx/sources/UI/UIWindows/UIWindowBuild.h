#pragma once

#include "../../../../EngineEditor/sources/Scene/SceneManager.h"
#include "UI/UIWindow.h"
#include "UI/UIWidgets.h"


class UIWindowBuild : public UIWindow
{
public:

	libgfx_API UIWindowBuild();
	//-------------------------------------------------------------------Other Functions--------------------------------------------------------------------
	//This function is called every frame to update the UI of this window
	void libgfx_API Update() override;

	//----------------------------------------------------------------This Window Related Functions------------------------------------------------------------------

	//TODO: Place this in parent class
	//This function will be executed if m_bDoOnce is = true, the idea is to use this function only once at the beginning of Update() to initialize stuff in this window only
	void DoOnceForThisWindow();

	libgfx_API bool IsSavesUpdated();

	void libgfx_API ResetSavesUpdated();
	libgfx_API std::vector<std::pair<std::string, std::string>>& GetXMLSaves();
	
private:
	//TODO: Place this in parent class
	//This boolean is used to call DoOnceForThisWindow() only once, it's initialized at true at the launching of the app and should never be reset to true during runtime
	bool m_bDoOnce = true;

	std::string m_chosenDirectoryPath;

	bool m_bSavesUpdated = false;

	std::vector<std::pair<std::string, std::string>> m_XMLSaves;
	//SceneManager* m_sceneManager;

};