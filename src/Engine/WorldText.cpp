#include "WorldText.h"

WorldText::~WorldText()
{
	delete m_Text;
	m_Text = nullptr;
}

void WorldText::OnInitialize() 
{
	m_IsWorldText = true;
}

void WorldText::OnUpdate()
{
	if (m_Text != nullptr) 
	{
		Transform2D& transform = GetTransform2D();
		Vector2f sizes = m_Text->GetSizes();
		m_Text->SetPosition(static_cast<int32>(m_RenderPosition.x - (sizes.x / 2.0f)), static_cast<int32>(m_RenderPosition.y - (sizes.y / 2.0f)));
	}
}

Text* WorldText::GetText() 
{
	return m_Text;
}

void WorldText::SetTextObject(Text* pText)
{
	if (m_Text != nullptr && m_Text != pText)
		delete m_Text;

	m_Text = pText;
}

void WorldText::SetRenderPosition(Vector2f pos) 
{
	m_RenderPosition = pos;
}

void WorldText::SetText(const std::string& text)
{
	if (m_Text == nullptr)
	{
		GCLE_WARN << "WorldText isn't initialize, can't use SetText" << ENDL;
		return;
	}
	m_Text->SetText(text);
}

void WorldText::SetFont(const std::string& id)
{
	if (m_Text == nullptr)
	{
		GCLE_WARN << "WorldText isn't initialize, can't use SetFont" << ENDL;
		return;
	}
	m_Text->SetFont(id);
}

void WorldText::SetTextColor(byte r, byte g, byte b, byte a)
{
	if (m_Text == nullptr)
	{
		GCLE_WARN << "WorldText isn't initialize, can't use SetTextColor" << ENDL;
		return;
	}
	m_Text->SetColor(r, g, b, a);
}

void WorldText::SetFontSize(int32 size)
{
	if (m_Text == nullptr)
	{
		GCLE_WARN << "WorldText isn't initialize, can't use SetFontSize" << ENDL;
		return;
	}
	m_Text->SetFontSize(size);
}

int32 WorldText::GetFontSize() const
{
	return m_Text != nullptr ? m_Text->GetFontSize() : 0;
}

Vector2f WorldText::GetTextSizes() const
{
	return m_Text != nullptr ? m_Text->GetSizes() : Vector2f{ 0.f, 0.f };
}

void WorldText::SetActive(bool active)
{
	GameObject::SetActive(active);
	m_Text->SetActive(active);
}
