#include "UIWindowManager.h"

#include <fstream>
#include <windows.h>
#include <iostream>

#include <GLFW/glfw3.h>

#include <imgui_internal.h>

#include "Defines.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

#include "ToolsEngine.h"
#include "../../../EngineEditor/sources/Scene/Scene.h"
#include "../../../EngineEditor/sources/Scene/SceneManager.h"
#include "../../../EngineEditor/sources/Scene/Actor.h"


libgfx_API UIWindowManager::UIWindowManager()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui::StyleColorsDark();

	SetStyle();

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	//io.ConfigDockingWithShift = true;

	m_UILayerIniFilePath = std::filesystem::current_path().string() + "\\resources\\DefaultUILayer.ini";
#ifdef _SHIPPING
	m_UIShipLayerIniFilePath = std::filesystem::current_path().string() + "\\resources\\ShippingUILayer.ini";
#endif

	m_ScriptFolderPath = std::filesystem::current_path().string() + "\\Assets\\Scripts";

	io.IniFilename = NULL;

#ifndef _SHIPPING
	ImGui::LoadIniSettingsFromDisk(m_UILayerIniFilePath.c_str());
#else
	ImGui::LoadIniSettingsFromDisk(m_UIShipLayerIniFilePath.c_str());
#endif
}

void UIWindowManager::Update(ImTextureID text)
{
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuiIO& io = ImGui::GetIO();

#ifndef _SHIPPING
	bool b = true;
	ShowDockSpace(&b);
#endif

	UpdateWindows(text);

	ImGui::Render();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void UIWindowManager::Render()
{
	// ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void libgfx_API UIWindowManager::AddWindow(UIWindow* newWin)
{
	m_windows.push_back(newWin);
}

std::vector<UIWindow*>& UIWindowManager::GetWindows()
{
	return m_windows;
}

libgfx_API UIWindow* UIWindowManager::GetWindow(std::string WindowName)
{
	for (UIWindow* window : m_windows)
	{
		if (window->GetWindowName() == WindowName)
		{
			return window;
		}
	}
	return nullptr;
}

void libgfx_API UIWindowManager::CLearWindows()
{
#ifndef _SHIPPING
	ImGui::SaveIniSettingsToDisk(m_UILayerIniFilePath.c_str());
#else
	ImGui::SaveIniSettingsToDisk(m_UIShipLayerIniFilePath.c_str());
#endif

	GraphicAPI* api = GraphicAPI::GetInstance();
	for (UIWindow* window : m_windows)
	{
		if (window->IsSamplerCreated() == true)
		{
			vkDestroySampler(api->GetDevice(), window->m_textureSampler.sampler, nullptr);
		}

		if (window->IsTextureVectorEmpty() == false)
		{
			for (UITexture texture : window->GetTextures())
			{
				texture.GetImage().DestroyImage();
			}
		}
	}
}

void UIWindowManager::SetStyle()
{
	ImGuiStyle* style = &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	colors[ImGuiCol_Text] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.500f, 0.500f, 0.500f, 1.000f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.180f, 0.180f, 0.180f, 1.000f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.280f, 0.280f, 0.280f, 0.000f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
	colors[ImGuiCol_Border] = ImVec4(0.266f, 0.266f, 0.266f, 1.000f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.200f, 0.200f, 0.200f, 1.000f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.280f, 0.280f, 0.280f, 1.000f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.148f, 0.148f, 0.148f, 1.000f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.160f, 0.160f, 0.160f, 1.000f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.277f, 0.277f, 0.277f, 1.000f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.300f, 0.300f, 0.300f, 1.000f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_CheckMark] = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_Button] = ImVec4(1.000f, 1.000f, 1.000f, 0.000f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
	colors[ImGuiCol_ButtonActive] = ImVec4(1.000f, 1.000f, 1.000f, 0.391f);
	colors[ImGuiCol_Header] = ImVec4(0.313f, 0.313f, 0.313f, 1.000f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_Separator] = colors[ImGuiCol_Border]; /*ImVec4(1.f, 0.f, 0.f, 1.f);*/
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.391f, 0.391f, 0.391f, 1.000f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(1.000f, 1.000f, 1.000f, 0.250f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(1.000f, 1.000f, 1.000f, 0.670f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_Tab] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.352f, 0.352f, 0.352f, 1.000f);
	colors[ImGuiCol_TabActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.098f, 0.098f, 0.098f, 1.000f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.195f, 0.195f, 0.195f, 1.000f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.469f, 0.469f, 0.469f, 1.000f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.586f, 0.586f, 0.586f, 1.000f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(1.000f, 1.000f, 1.000f, 0.156f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.000f, 0.391f, 0.000f, 1.000f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.000f, 0.000f, 0.000f, 0.586f);

	style->ChildRounding = 4.0f;
	style->FrameBorderSize = 1.0f;
	style->FrameRounding = 2.0f;
	style->GrabMinSize = 7.0f;
	style->PopupRounding = 2.0f;
	style->ScrollbarRounding = 12.0f;
	style->ScrollbarSize = 13.0f;
	style->TabBorderSize = 1.0f;
	style->TabRounding = 0.0f;
	style->WindowRounding = 4.0f;
}

void UIWindowManager::UpdateWindows(ImTextureID text)
{
	for (auto i = 0; i < m_windows.size(); i++)
	{
		if (m_windows[i] != nullptr)
		{
			if (m_windows[i]->GetWindowName() == "ViewportSceneWindow")
			{
				m_windows[i]->Update(text);
			}
			else
			{
				m_windows[i]->Update();
			}
		}
	}
}

void UIWindowManager::DeleteWindows()
{
}

void UIWindowManager::CreateMenu()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10.0f, 10.0f));

	std::string PopupAction = "";

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Add file", "CTRL+O"))
			{
				if (GetWindow("FileBrowserWindow") != nullptr)
				{
					GetWindow("FileBrowserWindow")->AddFileToCurrentFolder();
				}
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Build"))
		{
			m_bIsBuildWindowShowing = !m_bIsBuildWindowShowing;
			GetWindow("BuildWindow")->Show(m_bIsBuildWindowShowing);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("UIWindows"))
		{
			if (ImGui::BeginMenu("Movable Windows"))
			{
				if (ImGui::Checkbox("Make components window movable", &m_bIsComponentsWindowMovable))
				{
					ChangeWindowMovableState("ComponentsWindow");
				}

				if (ImGui::Checkbox("Make world hierarchy window movable", &m_bIsWorldHierarchyWindowMovable))
				{
					ChangeWindowMovableState("WorldHierarchyWindow");
				}

				if (ImGui::Checkbox("Make file browser window movable", &m_bIsFileBrowserWindowMovable))
				{
					ChangeWindowMovableState("FileBrowserWindow");
				}

				if (ImGui::Checkbox("Make viewport scene window movable", &m_bIsViewportSceneWindowMovable))
				{
					ChangeWindowMovableState("ViewportSceneWindow");
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Settings"))
			{
				ImGui::InputInt("Actor ID To Possess", &m_ActorIDToPossess);
				
				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("Show Demo Window", "CTRL+Y", false, true))
			{
				m_bShowDemoWindow = !m_bShowDemoWindow;
			}  // Disabled item

			if (ImGui::MenuItem("Create Lua Script", "CTRL+Y", false, true))
			{
				PopupAction = "Create Lua Script";
			}

			/*if (ImGui::MenuItem("Show UI Preview Window", "CTRL+Y", false, true))
			{
				if (GetWindow("UIPreviewWindow") != nullptr)
				{
					GetWindow("UIPreviewWindow")->Show(true);
				}
			}*/

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
	ImGui::PopStyleVar();

	if (m_bShowDemoWindow == true)
	{
		ImGui::ShowDemoWindow();
	}

	if (PopupAction == "Create Lua Script")
	{
		ImGui::OpenPopup("Create Lua Script Popup");

		if (m_CreationScriptName[0] != '\0')
		{
			m_CreationScriptName[0] = '\0';
		}
	}

    if (ImGui::BeginPopup("Create Lua Script Popup"))
    {
        if (ImGui::BeginCombo("Script Parent Type", m_chosenScriptTypeString.c_str()))
        {
            if (ImGui::Selectable("Character"))
            {
                m_chosenScriptType = ScriptType::CHARACTER;
                m_chosenScriptTypeString = "Character";
            }

            if (ImGui::Selectable("UI"))
            {
                m_chosenScriptType = ScriptType::UI;
                m_chosenScriptTypeString = "UI";
            }
            ImGui::EndCombo();
        }
            
        ImGui::Text("File Name :");
        ImGui::SameLine();
        ImGui::InputText("##dd", m_CreationScriptName, IM_ARRAYSIZE(m_CreationScriptName));

        if (ImGui::Button("Create Script"))
        {
        	if (m_CreationScriptName[0] == '\0')
        	{
                MessageBox(NULL, "Script name is empty, please make sure to enter a name for the script to create", "Unvalid script name", MB_OK);
        	}
            else if (m_chosenScriptTypeString.empty() == true)
            {
                MessageBox(NULL, "Script parent type is empty, please make sure to chose a parent for the script to create", "Unvalid script parent", MB_OK);
            }
            else
            {
                auto ScriptPath = m_ScriptFolderPath + "\\";

                std::string ScriptName = m_CreationScriptName;
                ScriptName.append(".lua");

                ScriptPath.append(ScriptName);
                    
                if (DoesFileExists(ScriptPath) == false)
                {
                    if (m_chosenScriptType == ScriptType::CHARACTER)
                    {
                        std::cout << "Script Character created";

                        ToolsEngine::CreateScriptCharacter(ScriptPath, ScriptName);

                        m_chosenScriptTypeString.clear();
                        ImGui::CloseCurrentPopup();
                    }
                    else if (m_chosenScriptType == ScriptType::UI)
                    {
                        std::cout << "Script UI created";

                        m_chosenScriptTypeString.clear();
                        ImGui::CloseCurrentPopup();
                    }
                }
                else
                {
                    MessageBox(NULL, "This file already exists, try to pick a different name", "File Name already used", MB_OK);
                }
            }   
        }
       
        ImGui::EndPopup();
    }
}


void UIWindowManager::ChangeWindowMovableState(std::string windowName)
{
	for (UIWindow* window : m_windows)
	{
		if (window->GetWindowName() == windowName)
		{
			window->SetWindowMovable(!window->IsWindowMovable());
		}
	}
}

bool UIWindowManager::DoesFileExists(const std::string& filename)
{
	struct stat buf;
	if (stat(filename.c_str(), &buf) != -1)
	{
		return true;
	}
	return false;
}

int libgfx_API UIWindowManager::GetActorIDToPossess()
{
	return m_ActorIDToPossess;
}

void UIWindowManager::SetActorIDToPossess(int id)
{
	m_ActorIDToPossess = id;
}

ImVec2 UIWindowManager::GetViewportSceneTopLeftPos()
{
	return m_viewportPositionTopLeft;
}

ImVec2 UIWindowManager::GetViewportSceneBottomRightPos()
{
	return m_viewportPositionBottomRight;
}

void UIWindowManager::ShowDockSpace(bool* p_open)
{
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_None/*ImGuiWindowFlags_MenuBar*/; /*| ImGuiWindowFlags_NoDocking;*/
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::SetNextWindowPos(ImVec2(0.f, 0.f));
		//TODO: Use the window's variable
		ImGui::SetNextWindowSize(ImVec2(1920.f, 1030.f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		window_flags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
	// and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	//
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("JVK Engine", p_open, window_flags);
#ifndef _SHIPPING
	CreateMenu();
#endif
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");

		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

		m_viewportPositionTopLeft = GetWindow("ViewportSceneWindow")->GetWindowSpaceTopLeftPos();
		m_viewportPositionBottomRight = GetWindow("ViewportSceneWindow")->GetWindowSpaceBottomRightPos();

		//if (!ImGui::DockBuilderGetNode(dockspace_id)) {
		//    ImGui::DockBuilderRemoveNode(dockspace_id);
		//    ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_None);

		//    ImGuiID dock_main_id = dockspace_id;

		//    ImGui::DockBuilderGetNode(dock_main_id);
		//    ImGuiID dock_up_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.05f, nullptr, &dock_main_id);
		//    ImGuiID dock_right_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.2f, nullptr, &dock_main_id);
		//    ImGuiID dock_left_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.2f, nullptr, &dock_main_id);
		//    ImGuiID dock_down_id = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.2f, nullptr, &dock_main_id);
		//    ImGuiID dock_down_right_id = ImGui::DockBuilderSplitNode(dock_down_id, ImGuiDir_Right, 0.6f, nullptr, &dock_down_id);

		//    ImGui::DockBuilderDockWindow("ComponentsWindow", dock_right_id);

		////     Disable tab bar for custom toolbar
		//   /* ImGuiDockNode* node = ImGui::DockBuilderGetNode(dock_up_id);
		//    node->LocalFlags |= ImGuiDockNodeFlags_NoTabBar;*/

		//    ImGui::DockBuilderFinish(dock_main_id);
		//}
	}
	else
	{
		//ShowDockingDisabledMessage();
	}

	ImGui::End();
}