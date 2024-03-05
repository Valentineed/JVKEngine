#include "UIWidgetText.h"

UIWidgetText::UIWidgetText(ImVec2 pos)
{
	m_pos = pos;
	m_typeString = "Text";
	m_type = WidgetsType::TEXT;
}

UIWidgetText::UIWidgetText(const char* text, Anchors anchor, ImVec2 anchorPosition, ImVec2 alignment, ImVec2 pos)
{
	strncpy(m_bufferText, text, 1000);
	m_pos = pos;
	m_typeString = "Text";
	m_type = WidgetsType::TEXT;

	m_anchor = anchor;
	m_anchorPosition = anchorPosition;
	m_alignment = alignment;
}

ImVec2 libgfx_API UIWidgetText::CalculateFinalPosition()
{
	float finalX = (GetAnchorPosition().x + GetPos().x) - (m_alignment.x * ImGui::CalcTextSize(m_bufferText).x);
	float finalY = (GetAnchorPosition().y + GetPos().y) - (m_alignment.y * ImGui::CalcTextSize(m_bufferText).y);

	return ImVec2(finalX, finalY);
}
