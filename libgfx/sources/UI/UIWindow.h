#pragma once
#include <libgfx_Export.h>
#include <string>
#include <filesystem>
#include <functional>

#include "imgui.h"
#include "TextureSampler.h"
#include "Resources/UITexture.h"

template <typename>
class  Delegate;

template <typename rType, typename... Args>
class Delegate<rType(Args ...)>
{
public:
	template<typename pClass>
	void Connect(pClass* t, rType(pClass::* method)(Args...))
	{
		m_function = [=](Args ... as) { (t->*method)(std::forward<Args>(as)...); };
	}

	rType operator() (Args... args)
	{
		return m_function(std::forward<Args>(args)...);
	}

protected:
	std::function<rType(Args ...)> m_function = {};
};

class UIWindow
{
public:
	//-------------------------------------------------------------------Constructors / Destructors-----------------------------------------------------------------
	UIWindow() = default;
	~UIWindow() = default;

	//-------------------------------------------------------------------Other Functions--------------------------------------------------------------------
	//This function is called every frame to update the UI of this window
	virtual libgfx_API void Update();
	virtual libgfx_API void Update(ImTextureID text);

	
	void Show(bool tf);

	//This functions return a std::string containing the window's name
	std::string& GetWindowName();

	//This function is used to set m_bIsWindowMovable, setting to false will prevent the window to move, setting to true make the window movable
	void SetWindowMovable(bool tf);

	//This functions returns a m_bIsWindowMovable. If it's true, it means the window is movable, if it's no, then the window cannot be moved
	bool IsWindowMovable() const;

	//This function adds a file to the current folder displayed in the file browser (Usually called within the main menu of the app)
	virtual libgfx_API void AddFileToCurrentFolder();

	bool IsSamplerCreated();

	bool IsTextureVectorEmpty();

	std::vector<UITexture>& GetTextures();

	void libgfx_API CreateSampler();
	
	//TODO: Remove this to a more generic location
	Sampler m_textureSampler;

	void SetWindowSpacePos();

	ImVec2& GetWindowSpaceTopLeftPos();
	ImVec2& GetWindowSpaceBottomRightPos();
protected:
	
	//TODO: Check if can be moved elsewhere
	virtual libgfx_API bool Begin(std::string name, ImGuiWindowFlags flags = 0);
	void End();


	//This bool is used to show the window. True will display the window, false won't
	bool m_bShow;

	//This bool is used to determine if the window can be moved by the user. True will make the window movable, false will prevent the window to move
	bool m_bIsWindowMovable = false;

	bool m_bIsTextureSamplerCreated = false;

	//This std::string contains the window's name
	std::string m_windowName;

	//This variable is the ImGui flags used for the window 
	ImGuiWindowFlags m_windowFlags = ImGuiWindowFlags_NoMove;

	//This boolean is used to call a sequence of functions only once for every child window, it's initialized at true at the launching of the app and should never be reset to true during runtime
	bool m_bDoOnceForEveryWindow = true;

	//Path towards the project path (from your computer root to JVK_Engine/)
	std::filesystem::path m_projectPath;

	//Path towards the "Assets" path (from your computer root to JVK_Engine/Assets)
	std::filesystem::path m_assetPath;

	//Path towards the "resources" path (from your computer root to JVK_Engine/resources)
	std::filesystem::path m_resourcesPath;

	std::vector<UITexture> m_textures;

	ImVec2 m_posTopLeftCorner;
	ImVec2 m_posBottomRightCorner;

	ImVec2 m_contentSize;


	
	//bool m_bWindow\ = false;
	

private:
};

