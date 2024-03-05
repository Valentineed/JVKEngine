#pragma once
#include "Resources/FolderAsset.h"
#include "Resources/FolderAsset.h"
#include "Resources/FolderAsset.h"
#include "Resources/FolderAsset.h"
#include "UI/UIWindow.h"

#include "TextureSampler.h"

class UIWindowFIleBrowser : public UIWindow
{
public:

	libgfx_API UIWindowFIleBrowser();
	//-------------------------------------------------------------------Other Functions--------------------------------------------------------------------
	
	//This function is called every frame to update the UI of this window
	void libgfx_API Update() override;

	//----------------------------------------------------------------This Window Related Functions------------------------------------------------------------------
	
	
	//TODO: Place this in parent class
	//This function will be executed if m_bDoOnce is == true, the idea is to use this function only once at the beginning of Update() to initialize stuff in this window only
	void DoOnceForThisWindow();

	//This function will be executed if m_bLoadAssets is == true, it loads all the asset present in the "Assets" folder
	void libgfx_API LoadAssets();

	//This function displays the thumbnail/name/tooltip of every files present in the "Assets" folder
	void DisplayFilesThumbnails();

	//This function displays the thumbnail/name/tooltip of every folders present in the "Assets" folder
	void DisplayFoldersThumbnails();

	//This function display the "back" button. Used to go back to the previous folder opened in the file browser.
	void DisplayBackToPreviousFolderButton();

	//This function displays a hierarchy of opened folders since the "Assets" folder (root)
	void DisplayCurrentFoldersHierarchy();

	//This function displays every files that match with the name filter the user wrote
	void DisplayFilteredFilesThumbnails(ImGuiTextFilter& filter);

	//This function display the given file
	void DisplayFile(FileAsset file, ImVec2 ThreeDotSize);

	//This function display the given folder
	void DisplayFolder(FolderAsset fold, ImVec2 ThreeDotSize);

	//This function displays a popup when we right click on a folder
	void CreateRightClickFolderPopUp(FolderAsset& fold);

	//This function displays a popup when we right click on a file
	void CreateRightClickFilePopUp(FileAsset& file);

	//This function adds a file to the current folder displayed in the file browser (Usually called within the main menu of the app)
	void  libgfx_API AddFileToCurrentFolder() override;

	//This functions clears the "Assets" vector and re-adds every assets
	void ReloadAssets();

	//TODO: Improve the delete of the image views
	void libgfx_API ClearImageViews();

	void GoToFolder(int folderID);

	libgfx_API std::string& GetLevelNameToLoad();

	void libgfx_API AddLevel(std::string name, std::string path);

	libgfx_API std::vector<std::pair<std::string, std::string>>& GetLevelsToAdd();

	libgfx_API bool HasNewLevelsToAdd();
private:
	//TODO: Place this in parent class
	//This boolean is used to call DoOnceForThisWindow() only once, it's initialized at true at the launching of the app and should never be reset to true during runtime
	bool m_bDoOnce = true;

	//This boolean is used to call LoadAssets() only once, it's initialized at true at the launching of the app to load the assets. Can be reset to true anytime we need to reload the
	//"Assets" folder
	bool m_bLoadAssets = true;

	//This vector contains all the folder present in the "Assets" folder
	std::vector<FolderAsset> m_folderAssets;

	//TODO: rename index into ID cause of IDFolder
	
	//This int is an index of which folder will be filled with the folders and files present in itself
	int m_indexCurrentDirectoryToFill = 0;

	//This int is an index of which folder's content will be displayed in the file browser
	int m_indexCurrentDirectoryToDisplay = 0;

	//This vector contains all the previous index of opened folders
	std::vector<int> m_previousIndexFolderOpened;

	std::string m_levelNameToLoad;

	std::pair<std::string, std::string> m_levelToAdd;

	std::vector<std::pair<std::string, std::string>> m_levelsToAdd;

	bool m_bHasNewLevelToAdd = false;

	std::string m_AssetNewName;
	std::string m_AssetNewNameBuf;

	bool m_bShouldCloseMainPopup = false;

	std::string m_currentAction;

};

