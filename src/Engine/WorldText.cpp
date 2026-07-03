#include "WorldText.h"

void WorldText::OnInitialize(){
	m_isWorldText = true;
}

void WorldText::OnUpdate(){
	if (m_text != nullptr) {
		Transform2D& transform = GetTransform2D();
		Vector2f sizes = m_text->GetSizes();
		m_text->SetPosition(static_cast<int32>(m_renderPosition.x - (sizes.x / 2.0f)), static_cast<int32>(m_renderPosition.y - (sizes.y / 2.0f)));
	}
}

void WorldText::SetRenderPosition(Vector2f pos){
	m_renderPosition = pos;
}
