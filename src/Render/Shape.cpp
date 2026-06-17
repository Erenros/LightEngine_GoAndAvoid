#include "Shape.h" 

void Shape::SetPosition(float32 x, float32 y, float32 ratioX, float32 ratioY)
{
	float32 dx = 0.f;
	float32 dy = 0.f;
	if (m_shape == Shapes::Circle) 
	{
		float32 size = m_radius * 2;

		x -= size * ratioX;
		y -= size * ratioY;

		dx = x - m_center.x;
		dy = y - m_center.y;
		m_center.x = x;
		m_center.y = y;

	}
	else if (m_shape == Shapes::Rectangle)
	{
		float32 sizeX = m_width * ratioX;
		float32 sizeY = m_height * ratioY;

		x -= sizeX;
		y -= sizeY;

		dx = x - m_verticies[0].position.x;
		dy = y - m_verticies[0].position.y;
	}
	else
	{
		dx = x - m_verticies[0].position.x;
		dy = y - m_verticies[0].position.y;
	}

	for (auto& vertex : m_verticies) {
		vertex.position.x += dx;
		vertex.position.y += dy;
	}
}