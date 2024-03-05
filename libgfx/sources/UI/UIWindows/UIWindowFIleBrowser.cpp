#include "UIWindowFIleBrowser.h"

#include <windows.h>
#include <WinUser.h>

#include <commdlg.h>

#include "ToolsEngine.h"

#include "Defines.h"
#include "UI/UIWidgets.h"


namespace fs = std::filesystem;

UIWindowFIleBrowser::UIWindowFIleBrowser()
{
    m_windowName = "FileBrowserWindow";
}

void libgfx_API UIWindowFIleBrowser::Update()
{
	UIWindow::Update();
	
    DoOnceForThisWindow();

    if (m_bLoadAssets == true)
    {
        if (m_folderAssets.empty() == false)
        {
            ClearImageViews();
            m_folderAssets.clear();
        	
        }
        LoadAssets();
        m_bLoadAssets = false;
    }
	

    if (m_bIsWindowMovable == true)
    {
        m_windowFlags = ImGuiWindowFlags_None;
    }
    else
    {
        m_windowFlags = ImGuiWindowFlags_NoMove;
    }

    //TODO: Change hard coding values later
    ImGui::SetNextWindowSizeConstraints(ImVec2(1600.f, 300.f), ImVec2(1600.f, 300.f));
	
    if (!Begin("FileBrowserWindow", m_windowFlags))
    {
        End();
    }
    else
    {

        SetWindowSpacePos();
    	
        if ((ImGui::GetMousePos().x >= m_posTopLeftCorner.x && ImGui::GetMousePos().x <= m_posBottomRightCorner.x) &&
            (ImGui::GetMousePos().y >= m_posTopLeftCorner.y && ImGui::GetMousePos().y <= m_posBottomRightCorner.y))
        {
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsAnyItemHovered() == false)
            {
                ImGui::OpenPopup("FileBrowserMainPopup");
            }
        }
    	
       
        if (ImGui::BeginPopup("FileBrowserMainPopup"))
        {
            ImGui::Separator();

            if (ImGui::Button("Add new level"))
            {
                ImGui::OpenPopup("RenameAsset");
            	m_currentAction = "Level";
            }

            if (ImGui::Button("Add directory"))
            {
                ImGui::OpenPopup("RenameAsset");
                m_currentAction = "Dir";
            }
            
            if (ImGui::BeginPopup("RenameAsset"))
            {
                if (ImGui::InputText("New name", (char*)m_AssetNewName.c_str(), 2048, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                	if (m_currentAction == "Level")
                	{
                        // m_AssetNewName = m_AssetNewNameBuf;
                        ToolsEngine::CreateNewLevel(m_AssetNewName);
                        ReloadAssets();
                        m_AssetNewName.clear();
                        // m_AssetNewNameBuf.clear();
                	}
                    else if (m_currentAction == "Dir")
                    {
                        std::error_code ec;
                        std::string filePath = m_folderAssets[m_indexCurrentDirectoryToDisplay].GetPath();
                    	filePath.append("\\");
                    	filePath.append(m_AssetNewName.c_str());
                    	
	                    if(std::filesystem::create_directory(filePath, ec) == false)
	                    {
                            std::cout << ec.message();
	                    }

                    	ReloadAssets();
                    	
                    }
                    m_currentAction.clear();
                    ImGui::CloseCurrentPopup();
                    m_bShouldCloseMainPopup = true;
                }
                ImGui::EndPopup();
            }
        	if (m_bShouldCloseMainPopup == true)
        	{
                m_bShouldCloseMainPopup = false;
                ImGui::CloseCurrentPopup();
        	}
           
            ImGui::EndPopup();
        }

    	
        DisplayBackToPreviousFolderButton();

        DisplayCurrentFoldersHierarchy();

       
        static ImGuiTextFilter filter;

        filter.Draw("##FileResearch");


        if (filter.IsActive() == false)
        {

            DisplayFoldersThumbnails();

            DisplayFilesThumbnails();
        }
        else
        {
            DisplayFilteredFilesThumbnails(filter);
        }

       /* if (ImGui::BeginPopupContextItem("RenameAsset"))
        {
            if (ImGui::InputText("New name", (char*)m_AssetNewNameBuf.c_str(), 2048), ImGuiInputTextFlags_EnterReturnsTrue)
            {
                m_AssetNewName = m_AssetNewNameBuf;

               ToolsEngine::CreateNewLevel(m_AssetNewName);
               ReloadAssets();
               m_AssetNewName.clear();
               m_AssetNewNameBuf.clear();
            }

            ImGui::EndPopup();
        }*/
        End();
    }
}

void UIWindowFIleBrowser::DoOnceForThisWindow()
{
    if (m_bDoOnce == true)
    {
        CreateSampler();
        m_bDoOnce = false;
    }
}

void UIWindowFIleBrowser::LoadAssets() 
{
    m_folderAssets.emplace_back(m_assetPath.string(), "N/A", m_resourcesPath.string(), m_indexCurrentDirectoryToFill, m_textureSampler);

    for (const auto& entry : fs::recursive_directory_iterator(m_assetPath))
    {
        std::size_t indexDot = entry.path().string().find_last_of('.');
        //if we find a dot in the path, it means it's a file, so we add it to the current directory or else it's a folder and we add a new folder to our directory vector
        if (indexDot != std::string::npos && indexDot == entry.path().string().length() - 4)
        {        	
            m_indexCurrentDirectoryToFill = 0;

            for each (auto folder in m_folderAssets)
            {
                if (folder.GetPath() == entry.path().parent_path())
                {
                    m_folderAssets[m_indexCurrentDirectoryToFill].AddFile(entry.path().string(), m_projectPath.string(), m_textureSampler);
                	if (entry.path().extension().string() == ".xml")
                	{
                        std::string filename = entry.path().filename().string();
                        filename = filename.substr(0, filename.size() - 4);
                        AddLevel(filename, entry.path().string());
                	}
                }
            	else
            	{
                    m_indexCurrentDirectoryToFill++;
            	}
            }
        }
        else
        {
            if (!m_folderAssets.empty())
            {
                m_indexCurrentDirectoryToFill++;
            }
            m_folderAssets.emplace_back(entry.path().string(), entry.path().parent_path().string(), m_resourcesPath.string(), m_indexCurrentDirectoryToFill, m_textureSampler);
        }

    }

	if (m_levelsToAdd.empty() == false)
	{
        m_bHasNewLevelToAdd = true;
	}
}

void UIWindowFIleBrowser::DisplayFilesThumbnails()
{
    //TODO: Check if it's possible to drag&drop image later because of the selectable

    ImVec2 ThreeDotsTextSize = ImGui::CalcTextSize("...");


    std::vector<FileAsset> fileAsset = m_folderAssets[m_indexCurrentDirectoryToDisplay].GetAssetFiles();
    if (!fileAsset.empty())
    {
        for (FileAsset file : m_folderAssets[m_indexCurrentDirectoryToDisplay].GetAssetFiles())
        {
            DisplayFile(file, ThreeDotsTextSize);
        }
    }
}

void UIWindowFIleBrowser::DisplayFoldersThumbnails()
{
    ImVec2 ThreeDotsTextSize = ImGui::CalcTextSize("...");
    for (FolderAsset fold : m_folderAssets)
    {
        DisplayFolder(fold, ThreeDotsTextSize);
    }
}

void UIWindowFIleBrowser::DisplayBackToPreviousFolderButton()
{
    if (ImGui::Button("Back") && m_previousIndexFolderOpened.size() != 0)
    {
        m_indexCurrentDirectoryToDisplay = m_previousIndexFolderOpened.back();
        m_previousIndexFolderOpened.pop_back();
    }

	ImGui::SameLine();
	
    if (ImGui::Button("Reload Assets"))
    {
       ReloadAssets();
    }

}

void UIWindowFIleBrowser::DisplayCurrentFoldersHierarchy()
{
    if (m_indexCurrentDirectoryToDisplay == 0)
    {
        ImGui::Button(m_folderAssets[m_indexCurrentDirectoryToDisplay].GetName().c_str());
    }
    else
    {
        for (int previousIndex : m_previousIndexFolderOpened)
        {

            if (ImGui::Button(m_folderAssets[previousIndex].GetName().c_str()))
            {
                m_indexCurrentDirectoryToDisplay = m_folderAssets[previousIndex].GetFolderID();
                auto it = std::find(m_previousIndexFolderOpened.begin(), m_previousIndexFolderOpened.end(), m_indexCurrentDirectoryToDisplay);
                m_previousIndexFolderOpened.erase(it, m_previousIndexFolderOpened.end());
                return;
            }


            ImGui::SameLine();

            IUIWidgets::CreateText(" > ");

            ImGui::SameLine();
            if (previousIndex == m_previousIndexFolderOpened.back())
            {
                ImGui::Button(m_folderAssets[m_indexCurrentDirectoryToDisplay].GetName().c_str());
            }
        }
    }
}

void UIWindowFIleBrowser::DisplayFilteredFilesThumbnails(ImGuiTextFilter& filter)
{
    ImVec2 ThreeDotsTextSize = ImGui::CalcTextSize("...");
	
    for (int i = 0; i < m_folderAssets.size(); i++)
    {
        if (!m_folderAssets[i].GetAssetFiles().empty())
        {
	        for (FileAsset file : m_folderAssets[i].GetAssetFiles())
	        {
                if (filter.PassFilter(file.GetName().c_str()) == true)
                {
                    DisplayFile(file, ThreeDotsTextSize);
                }
	        }
        }
    }
}

void UIWindowFIleBrowser::DisplayFile(FileAsset file, ImVec2 ThreeDotSize)
{
    ImVec2 SelectableFilePostion = ImGui::GetCursorPos();
    ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, UI_FILEBROWSER_ALIGNMENT_FILENAME);
    ImVec2 FileNameTextSize = ImGui::CalcTextSize(file.GetNameWithExt().c_str());
    if (FileNameTextSize.x > UI_FILEBROWSER_THUMBNAIL_SIZE.x)
    {
        std::string ClippedName = file.GetNameWithExt();
        while (ImGui::CalcTextSize(ClippedName.c_str()).x >= UI_FILEBROWSER_THUMBNAIL_SIZE.x - ThreeDotSize.x)
        {
            ClippedName.pop_back();
        }
        ClippedName.append("...");
        file.SetDisplayName(ClippedName);
    }
    //TODO: Reput the file name
    if (ImGui::Selectable(file.GetDisplayName().c_str(), false, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(UI_FILEBROWSER_THUMBNAIL_SIZE.x, UI_FILEBROWSER_THUMBNAIL_SIZE.y)))
    {
        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
        {
            if (file.GetExtension() == "xml")
            {
                m_levelNameToLoad = file.GetName();
            }
        }
    }

    if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
    {
        ImGui::OpenPopup(file.GetName().c_str());
    }

    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(file.GetNameWithExt().c_str());
    }

    ImGui::PopStyleVar();
    ImGui::SetCursorPos(SelectableFilePostion);
    ImGui::Image((void*)(intptr_t)file.GetTextureID(), ImVec2(UI_FILEBROWSER_THUMBNAIL_SIZE.x, UI_FILEBROWSER_THUMBNAIL_SIZE.y));
    ImGui::SameLine();

    CreateRightClickFilePopUp(file);
}

void UIWindowFIleBrowser::DisplayFolder(FolderAsset fold, ImVec2 ThreeDotSize)
{
    if (fold.GetParentPath() == m_folderAssets[m_indexCurrentDirectoryToDisplay].GetPath())
    {

        ImGui::PushStyleVar(ImGuiStyleVar_SelectableTextAlign, UI_FILEBROWSER_ALIGNMENT_FILENAME);
        ImVec2 FolderNameTextSize = ImGui::CalcTextSize(fold.GetName().c_str());

        if (FolderNameTextSize.x > UI_FILEBROWSER_THUMBNAIL_SIZE.x)
        {
            std::string ClippedName = fold.GetName();
            while (ImGui::CalcTextSize(ClippedName.c_str()).x >= UI_FILEBROWSER_THUMBNAIL_SIZE.x - ThreeDotSize.x)
            {
                ClippedName.pop_back();
            }
            ClippedName.append("...");
            fold.SetDisplayName(ClippedName);
        }
        ImVec2 SelectablePostion = ImGui::GetCursorPos();
        //TODO : Look for apply tooltip[ when title is hovered and add a lil timer

        if (ImGui::Selectable(fold.GetDisplayName().c_str(), false, ImGuiSelectableFlags_AllowDoubleClick, ImVec2(UI_FILEBROWSER_THUMBNAIL_SIZE.x, UI_FILEBROWSER_THUMBNAIL_SIZE.y)))
        {
            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                m_previousIndexFolderOpened.emplace_back(m_indexCurrentDirectoryToDisplay);
                m_indexCurrentDirectoryToDisplay = fold.GetFolderID();
                ImGui::PopStyleVar();
                return;
            }
        }
        if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
        {
            ImGui::OpenPopup(fold.GetName().c_str());
        }

        if (ImGui::IsItemHovered())
        {
            ImGui::SetTooltip(fold.GetName().c_str());
        }
        ImGui::PopStyleVar();
        ImGui::SetCursorPos(SelectablePostion);
        ImGui::Image((void*)(intptr_t)fold.GetTextureID(), ImVec2(UI_FILEBROWSER_THUMBNAIL_SIZE.x, UI_FILEBROWSER_THUMBNAIL_SIZE.y));
        ImGui::SameLine();

        CreateRightClickFolderPopUp(fold);
    }
}

void UIWindowFIleBrowser::CreateRightClickFolderPopUp(FolderAsset& fold)
{
    if (ImGui::BeginPopup(fold.GetName().c_str()))
    {
        if (ImGui::Selectable("Open folder in windows explorer"))
        {
            ShellExecute(nullptr, "open", fold.GetPath().c_str(), NULL, NULL, SW_SHOWDEFAULT);
        }

        ImGui::EndPopup();
    }
}

void UIWindowFIleBrowser::CreateRightClickFilePopUp(FileAsset& file)
{
    if (ImGui::BeginPopup(file.GetName().c_str()))
    {
        ImGui::Separator();
        if (ImGui::Selectable("Open file in windows explorer"))
        {
            ShellExecute(nullptr, "open", file.GetPath().c_str(), NULL, NULL, SW_SHOWDEFAULT);
        }

        if (ImGui::Selectable("Delete file"))
        {
            std::filesystem::remove(file.GetPath());
            ReloadAssets();	
        }

        ImGui::EndPopup();
    }
}

void libgfx_API UIWindowFIleBrowser::AddFileToCurrentFolder()
{
    std::string PathFound = ToolsEngine::OpenFileName();

    if (PathFound.empty() == false)
    {
        fs::copy(PathFound, m_folderAssets[m_indexCurrentDirectoryToDisplay].GetPath());
        ReloadAssets();
    }
}

void UIWindowFIleBrowser::ReloadAssets()
{
    m_bLoadAssets = true;
    m_indexCurrentDirectoryToFill = 0;
}

void libgfx_API UIWindowFIleBrowser::ClearImageViews()
{
	for (FolderAsset folder : m_folderAssets)
	{
		if (folder.GetAssetFiles().empty() == false)
		{
			for (FileAsset file : folder.GetAssetFiles())
			{
                file.DestroyDefaultImage();
                if (file.IsOnDefaultTexture() == false)
                {
                    file.DestroyImage();
                }
			}
		}
        folder.DestroyImage();
	}
}

std::string& UIWindowFIleBrowser::GetLevelNameToLoad()
{
    return m_levelNameToLoad;
}

void UIWindowFIleBrowser::AddLevel(std::string name, std::string path)
{
    m_levelsToAdd.push_back({ name, path });
}

std::vector<std::pair<std::string, std::string>>& UIWindowFIleBrowser::GetLevelsToAdd()
{
    return m_levelsToAdd;;
}

bool UIWindowFIleBrowser::HasNewLevelsToAdd()
{
    return m_bHasNewLevelToAdd;
}
    
