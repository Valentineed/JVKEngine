#include "UIWindowBuild.h"

#include "UIWindowViewportScene.h"
#include "Image.h"
#include "UI/UIWidgets.h"
#include "Resources/TextureLoader.h"
#include <windows.h>
#include <imgui_internal.h>

#include "ToolsEngine.h"
#include "../../../../EngineEditor/sources/Scene/SceneManager.h"

UIWindowBuild::UIWindowBuild()
{
	m_windowName = "BuildWindow";
}

void libgfx_API UIWindowBuild::Update()
{
	UIWindow::Update();
	DoOnceForThisWindow();
	
	if (m_bShow == true)
	{
		//TODO: Change hard coding values later
		if (!Begin("BuildWindow"))
		{
			End();
		}
		else
		{
			ImGui::InputText("Directory Path", (char*)m_chosenDirectoryPath.c_str(), 2048, ImGuiInputTextFlags_ReadOnly);
			ImGui::SameLine();
			if (ImGui::Button("Chose path"))
			{
				m_chosenDirectoryPath = ToolsEngine::ChoseFolderPath();
				
			}

			ImGuiTableFlags flags = ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_BordersOuterV;

			if (ImGui::BeginTable("##SavesTable", 1, flags))
			{				
				for (std::pair<std::string_view, std::string_view> pair : m_XMLSaves)
				{
					ImGui::TableNextRow();
					ImGui::TableSetColumnIndex(0);
					ImGui::Text(pair.second.data());
					
				}
				ImGui::EndTable();
			}
			
			if (ImGui::Button("Add level"))
			{
				std::string PathFound = ToolsEngine::OpenFileName();

				if (PathFound.empty() == false)
				{
					if (PathFound.find(".xml") != std::string::npos) {
						/*std::string PathCopy = PathFound;
						PathCopy = PathCopy.substr(0, PathCopy.size() - 4);*/
						std::filesystem::path fileName = PathFound;
						fileName = fileName.filename();

						std::string filenamestring = fileName.filename().string();

						filenamestring = filenamestring.substr(0, filenamestring.size() - 4);

						m_XMLSaves.push_back(std::make_pair(filenamestring, PathFound));
						
						m_bSavesUpdated = true;

					}
					else
					{
						MessageBox(NULL, "The path given was not a xml file", "Error adding level", MB_OK);
					}
				}

				//ToolsEngine::ReadBatchFile(BATCH_FILE);
			}
			
			if (ImGui::Button("Launch Batch File"))
			{
				if (m_chosenDirectoryPath.empty() == false && m_XMLSaves.empty() == false)
				{
					ToolsEngine::RewriteBatShippingFile(m_chosenDirectoryPath);
					ToolsEngine::ReadBatchFile(BATCH_FILE);
				}
				else
				{
					MessageBox(NULL, "Directory Path is empty or no level have been added", "Error building", MB_OK);
				}
				
				

				
			}

			End();
		}
	}
	
}

void UIWindowBuild::DoOnceForThisWindow()
{
	if (m_bDoOnce == true)
	{
		CreateSampler();

		m_bShow = false;
		
		m_bDoOnce = false;
	}
}

bool UIWindowBuild::IsSavesUpdated()
{
	return m_bSavesUpdated;
}

void libgfx_API UIWindowBuild::ResetSavesUpdated()
{
	m_bSavesUpdated = false;
}

std::vector<std::pair<std::string, std::string>>& UIWindowBuild::GetXMLSaves()
{
	return m_XMLSaves;
}
