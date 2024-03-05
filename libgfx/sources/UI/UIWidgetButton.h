#pragma once
#include "UIWidgets.h"

class UIWidgetButton : public UIWidgets
{
public:
	libgfx_API ImVec2& GetSize();
	libgfx_API void SetSize(ImVec2& size);

	libgfx_API std::string& GetButtonLabel();
	void SetButtonLabel(std::string& text);
private:
	ImVec2 m_size;

	std::string m_buttonLabel = "";
};

