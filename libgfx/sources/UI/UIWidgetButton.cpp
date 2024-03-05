#include "UIWidgetButton.h"

libgfx_API ImVec2& UIWidgetButton::GetSize()
{
	return m_size;
}

libgfx_API void UIWidgetButton::SetSize(ImVec2& size)
{
	m_size = size;
}

libgfx_API std::string& UIWidgetButton::GetButtonLabel()
{
	return m_buttonLabel;
}

void UIWidgetButton::SetButtonLabel(std::string& text)
{
	m_buttonLabel = text;
}
