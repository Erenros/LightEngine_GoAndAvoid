#include "Shape.h"

void Rectangle::Draw(Window* pWindow) {
	SDL_RenderGeometry(pWindow->GetRenderer(), GetTexture(), m_vertices, m_vertexNbr, m_rectangleIndices, m_indicesNbr);
}


void Triangle::Draw(Window* pWindow) {
	SDL_RenderGeometry(pWindow->GetRenderer(), GetTexture(), m_vertices, m_vertexNbr, nullptr, m_indicesNbr);
}


void Circle::Draw(Window* pWindow) {
	SDL_RenderGeometry(pWindow->GetRenderer(), GetTexture(), m_vertices, m_vertexNbr, m_indices, m_indicesNbr);
}

