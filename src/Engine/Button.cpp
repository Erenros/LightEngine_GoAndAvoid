#include "Button.h"


Button::Button()
{
}

void Button::Click()
{
	for (int32 i = 0; i < m_FunctionToCallOnActivate.size(); i++) {
		m_FunctionToCallOnActivate[i]();
	}
	OnClick();
}
