#include "UIWidgets.h"

#include <filesystem>

#include "Defines.h"
#include "imgui_impl_vulkan.h"
#include "Resources/TextureLoader.h"
#include "Resources/ActorComponentsData.h"


ImVec2& UIWidgets::GetPos()
{
	return m_pos;
}

libgfx_API void UIWidgets::SetPos(ImVec2& pos)
{
	m_pos = pos;
}

WidgetsType UIWidgets::GetType()
{
	return m_type;
}

std::string& UIWidgets::GetTypeString()
{
	return m_typeString;
}

libgfx_API bool& UIWidgets::IsVisible()
{
	return m_bIsVisible;
}

libgfx_API void UIWidgets::SetIsVisible(bool tf)
{
	m_bIsVisible = tf;
}

libgfx_API Anchors& UIWidgets::GetAnchor()
{
	return m_anchor;
}

libgfx_API void UIWidgets::SetAnchorVP(Anchors anchor, ImVec2 viewportTopLeft, ImVec2 viewportTopRight, ImVec2 viewportBotLeft, ImVec2 viewportBotRight)
{
	m_anchor = anchor;

	switch (anchor)
	{
		case Anchors::TOP_LEFT:
			m_anchorPosition = viewportTopLeft;
			break;
		case Anchors::TOP_MIDDLE:
			m_anchorPosition = ImVec2(viewportTopRight.x / 2, viewportTopRight.y);
			break;
		case Anchors::TOP_RIGHT:
			m_anchorPosition = viewportTopRight;
			break;
		case Anchors::MIDDLE_LEFT:
			m_anchorPosition = ImVec2(viewportTopLeft.x, viewportBotLeft.y / 2);
			break;
		case Anchors::MIDDLE:
			m_anchorPosition = ImVec2(viewportBotRight.x / 2, viewportBotRight.y / 2);
			break;
		case Anchors::MIDDLE_RIGHT:
			m_anchorPosition = ImVec2(viewportTopRight.x, viewportBotRight.y / 2);
			break;
		case Anchors::BOTTOM_LEFT:
			m_anchorPosition = viewportBotLeft;
			break;
		case Anchors::BOTTOM_MIDDLE:
			m_anchorPosition = ImVec2(viewportBotRight.x / 2, viewportBotRight.y);
			break;
		case Anchors::BOTTOM_RIGHT:
			m_anchorPosition = viewportBotRight;
			break;
	}
	
}

libgfx_API void UIWidgets::SetAnchor(Anchors anchor)
{
	m_anchor = anchor;
}

libgfx_API ImVec2& UIWidgets::GetAnchorPosition()
{
	return m_anchorPosition;
}


libgfx_API ImVec2& UIWidgets::GetAlignment()
{
	return m_alignment;
}

void libgfx_API UIWidgets::SetAlignment(ImVec2& alignment)
{
	m_alignment = alignment;
}

bool IUIWidgets::CreateHeaderComponent(const std::string& label)
{
	ImU32 cell_bg_color = ImGui::GetColorU32(UI_COMPONENTS_TITLE_CELL_BG_COLOR);
	ImGui::TableSetColumnIndex(0);
	bool open = ImGui::TreeNodeEx(label.c_str(), ImGuiTreeNodeFlags_SpanFullWidth);
	ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
	ImGui::TableSetColumnIndex(1);
	ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);

	return open;
}

void IUIWidgets::CreateComponentContentTitle(const std::string& label, bool hoveredToolTip, const std::string& labelTooltip)
{
	ImGui::TableNextRow();
	ImGui::TableSetColumnIndex(0);
	ImGui::Text(label.c_str());
	if (hoveredToolTip == true)
	{
		if (ImGui::IsItemHovered())
		{
			ImGui::SetTooltip(labelTooltip.c_str());
		}
	}
	
	ImGui::TableSetColumnIndex(1);
}

void IUIWidgets::CreateVec3Content(ImTextureID textureX, ImTextureID textureY, ImTextureID textureZ, const std::string& labelX, const std::string& labelY, const std::string& labelZ, Vector3& transformContent, bool& updated)
{
	ImGui::Image(textureX, ImVec2(20, 20));
	ImGui::SameLine();
	ImGui::PushItemWidth(50.f);
	if (ImGui::InputFloat(labelX.c_str(), &transformContent.x, 0, 0, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
	{
		updated = true;
	}
	ImGui::SameLine();
	ImGui::Image(textureY, ImVec2(20, 20));
	ImGui::SameLine();
	if (ImGui::InputFloat(labelY.c_str(), &transformContent.y, 0, 0, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
	{
		updated = true;
	}
	ImGui::SameLine();
	ImGui::Image(textureZ, ImVec2(20, 20));
	ImGui::SameLine();
	if (ImGui::InputFloat(labelZ.c_str(), &transformContent.z, 0, 0, "%.3f", ImGuiInputTextFlags_EnterReturnsTrue))
	{
		updated = true;
	}
	ImGui::PopItemWidth();
}

void IUIWidgets::CreateVec3Content(ImTextureID textureX, ImTextureID textureY, ImTextureID textureZ, const std::string& labelX, const std::string& labelY, const std::string& labelZ, Vector4& transformContent, bool& updated)
{
	ImGui::Image(textureX, ImVec2(20, 20));
	ImGui::SameLine();
	ImGui::PushItemWidth(50.f);
	if (ImGui::InputFloat(labelX.c_str(), &transformContent.x))
	{
		updated = true;
	}
	ImGui::SameLine();
	ImGui::Image(textureY, ImVec2(20, 20));
	ImGui::SameLine();
	if (ImGui::InputFloat(labelY.c_str(), &transformContent.y))
	{
		updated = true;
	}
	ImGui::SameLine();
	ImGui::Image(textureZ, ImVec2(20, 20));
	ImGui::SameLine();
	if (ImGui::InputFloat(labelZ.c_str(), &transformContent.z))
	{
		updated = true;
	}
}


void IUIWidgets::CreateColorButtonAndPickerOnPopup(const std::string& labelColorButton, const std::string& labelColorPicker, const std::string& labelPopup, ImVec4& vec4Color, bool& updated)
{
	if (ImGui::ColorButton(labelColorButton.c_str(), vec4Color))
	{
		if (ImGui::IsPopupOpen(labelPopup.c_str()) == false)
		{
			ImGui::OpenPopup(labelPopup.c_str());
		}
		else
		{
			ImGui::CloseCurrentPopup();
		}
	}

	if (ImGui::BeginPopup(labelPopup.c_str()))
	{
		if (ImGui::ColorPicker4(labelColorPicker.c_str(), (float*)&vec4Color, ImGuiColorEditFlags_NoAlpha))
		{
			updated = true;
		}
		ImGui::EndPopup();
	}
}

void IUIWidgets::CreateText(const std::string& text)
{
	ImGui::Text(text.c_str());
}

void IUIWidgets::OnRightClickComponent(const std::string& labelComponent, std::string& componentNameToRemove)
{
	if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
	{
		ImGui::OpenPopup(labelComponent.c_str());
	}

	if (ImGui::BeginPopup(labelComponent.c_str()))
	{
		if (ImGui::Selectable("Remove this component"))
		{
			componentNameToRemove.clear();
			componentNameToRemove = labelComponent;
		}
		ImGui::EndPopup();
	}
}
