#pragma once
#include <functional>
#include <string>

#include "imgui.h"
#include <Image.h>
#include <TextureSampler.h>

class ActorComponentsData;

enum class WidgetsType
{
    NONE = 69,
    TEXT = 0,
    IMAGE,
	BUTTON
};

enum class Anchors
{
    TOP_LEFT = 0,
    TOP_MIDDLE,
	TOP_RIGHT,
	MIDDLE_LEFT,
	MIDDLE,
	MIDDLE_RIGHT,
	BOTTOM_LEFT,
	BOTTOM_MIDDLE,
	BOTTOM_RIGHT
};

class UIWidgets
{
public:
    UIWidgets() = default;
    ~UIWidgets() = default;
    libgfx_API ImVec2& GetPos();
    libgfx_API void SetPos(ImVec2& pos);
    libgfx_API WidgetsType GetType();
    libgfx_API std::string& GetTypeString();
    libgfx_API bool& IsVisible();
    libgfx_API void SetIsVisible(bool tf);

	
    libgfx_API Anchors& GetAnchor();
    libgfx_API void SetAnchorVP(Anchors anchor, ImVec2 viewportTopLeft, ImVec2 viewportTopRight, ImVec2 viewportBotLeft, ImVec2 viewportBotRight);
    libgfx_API void SetAnchor(Anchors anchor);

    libgfx_API ImVec2& GetAnchorPosition();

    libgfx_API ImVec2& GetAlignment();
    void libgfx_API SetAlignment(ImVec2& alignment);
protected:
    ImVec2 m_pos;
    WidgetsType m_type;
    std::string m_typeString;
    bool m_bIsVisible = true;

    Anchors m_anchor = Anchors::TOP_LEFT;
    ImVec2 m_anchorPosition;

    ImVec2 m_alignment = ImVec2(0.f, 0.f);
};


class IUIWidgets
{
public:
    template<typename pClass>
	static void CreateViewportButton(const std::string_view& label, bool& isButtonPressed, ImVec4& ButtonColor, ImTextureID& texture, pClass* pclass, void(pClass::* logic)(bool& isButtonPressed, ImVec4& ButtonColor));

    template<typename pClass>
    static void CreateBasicButton(const std::string_view& label, ImVec2 size, pClass* pclass, void(pClass::* logic)());

    static bool CreateHeaderComponent(const std::string& label);

    static void CreateComponentContentTitle(const std::string& label, bool hoveredToolTip = false, const std::string& labelTooltip = "Tooltip to modify");

    static void CreateVec3Content(ImTextureID textureX, ImTextureID textureY, ImTextureID textureZ, const std::string& labelX, const std::string& labelY, const std::string& labelZ, Vector3& transformContent, bool& updated);
		
    static void CreateVec3Content(ImTextureID textureX, ImTextureID textureY, ImTextureID textureZ, const std::string& labelX, const std::string& labelY, const std::string& labelZ, Vector4& transformContent, bool& updated);
	
    static void CreateColorButtonAndPickerOnPopup(const std::string& labelColorButton, const std::string& labelColorPicker, const std::string& labelPopup, ImVec4& vec4Color, bool& updated);
	
    static void CreateText(const std::string& text);

    static void OnRightClickComponent(const std::string& labelComponent, std::string& componentNameToRemove);
private:
	std::function<void()> m_test;
};


template<typename pClass>
void IUIWidgets::CreateViewportButton(const std::string_view& label, bool& isButtonPressed, ImVec4& ButtonColor, ImTextureID& texture, pClass* pclass, void(pClass::* logic)(bool& isButtonPressed, ImVec4& ButtonColor))
{
    auto PlayButtonPos = ImGui::GetCursorPos();
    if (ImGui::Button(label.data(), ImVec2(20.f, 20.f)))
    {
        (pclass->*logic)(isButtonPressed, ButtonColor);
    }

    ImGui::SetCursorPos(PlayButtonPos);
    ImGui::Image(texture, ImVec2(20.f, 20.f), ImVec2(0, 0), ImVec2(1, 1), ButtonColor);
}

template<typename pClass>
void IUIWidgets::CreateBasicButton(const std::string_view& label, ImVec2 size, pClass* pclass, void(pClass::* logic)())
{
    if (ImGui::Button(label.data(), size))
    {
        (pclass->*logic)();
    }
}

