#pragma once
#include "UIWidgets.h"

class UIWidgetText : public UIWidgets
{
public:

	UIWidgetText(ImVec2 pos = ImVec2(0.f, 0.f));
	UIWidgetText(const char* text, Anchors anchor, ImVec2 anchorPosition, ImVec2 alignment, ImVec2 pos = ImVec2(0.f, 0.f));
	char m_bufferText[1000] = "Type your text here.";

	ImVec2 libgfx_API CalculateFinalPosition();
private:
	
};

