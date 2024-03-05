#pragma once

#include <vector>


#include "../../../EngineEditor/sources/Components/ScriptComponent.h"
#include "UI/UIWindow.h"

class UIWindowManager {
public:
	//-------------------------------------------------------------------Constructors / Destructors--------------------------------------------------------------------
    libgfx_API UIWindowManager();
    //UIWindowManager(GLFWwindow* window);
   // ~UIWindowManager() = default;
   
   
   //-------------------------------------------------------------------Other Functions--------------------------------------------------------------------

	//This function updates the UI of the engine
    void Update(ImTextureID text);
	
	//TODO: Look to replace this with the vulkan equivalent and use it
    void Render();

	//This function adds a new UI window to display
    void libgfx_API AddWindow(UIWindow* newWin);

	//This function returns the vector containing all the UI windows
    std::vector<UIWindow*>& GetWindows();

	//This function returns a UI Window with the given name
    libgfx_API UIWindow* GetWindow(std::string WindowName);

	//TODO: Remove this when we have a generic sampler
    void libgfx_API CLearWindows();

    // Function: fileExists
   /**
       Check if a file exists
   @param[in] filename - the name of the file to check

   @return    true if the file exists, else false

   */
    bool DoesFileExists(const std::string& filename);

    int libgfx_API GetActorIDToPossess();
    void libgfx_API SetActorIDToPossess(int id);

    ImVec2 libgfx_API GetViewportSceneTopLeftPos();
    ImVec2 libgfx_API GetViewportSceneBottomRightPos();
private:
	//This vector contains all the UI window to display in the editor
    std::vector<UIWindow*> m_windows;

	//This function creates the dockspace of the editor
    void ShowDockSpace(bool* p_open);

	//This function set the style of every UI window
    void SetStyle();

	//This function calls every Update() function of every window present in the windows vector
    void UpdateWindows(ImTextureID text);

	//TODO: See if using this function is usefull or not (can be used to delete a window during runtime)
    void DeleteWindows();

	//This function creates the main menu of the editor
    void CreateMenu();

	//This function will make a window movable. It looks through all the window and find the window with the given name to make it movable
    void ChangeWindowMovableState(std::string windowName);

	//This bool is used for the setting's checkbox to see if the components window is movable or not
    bool m_bIsComponentsWindowMovable = false;
	
    //This bool is used for the setting's checkbox to see if the world hierarchy window is movable or not
    bool m_bIsWorldHierarchyWindowMovable = false;

    //This bool is used for the setting's checkbox to see if the file browser window is movable or not
    bool m_bIsFileBrowserWindowMovable = false;

    //This bool is used for the setting's checkbox to see if the file browser window is movable or not
    bool m_bIsViewportSceneWindowMovable = false;

    bool m_bShowDemoWindow = false;
    bool m_bIsBuildWindowShowing = false;

    std::string m_UILayerIniFilePath;
    std::string m_UIShipLayerIniFilePath;
    std::string m_ScriptFolderPath;
    char m_CreationScriptName[100] = { 0 };

    int m_ActorIDToPossess = -1;

    ImVec2 m_viewportPositionTopLeft;
    ImVec2 m_viewportPositionBottomRight;

    ScriptType m_chosenScriptType;
    std::string m_chosenScriptTypeString;
};