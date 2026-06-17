#include "Shape.h" 

void Shape::SetPosition(float32 x, float32 y) {
	
	float32 dx;
	float32 dy;
	if (m_shape == Shapes::Circle) {
		dx = x - m_center.x;
		dy = y - m_center.y;
	}
	else {
		dx = x - m_verticies[0].position.x;
		dy = y - m_verticies[0].position.y;
	}

	for (auto& vertex : m_verticies) {
		vertex.position.x += dx;
		vertex.position.y += dy;
	}
}