#include "Shape.h" 

namespace gcle
{
	Shape::Shape(Shape* pShape) : m_texture(pShape->m_texture), m_Transform(pShape->m_Transform), m_radius(pShape->m_radius), m_center(pShape->m_center),
		smoothness(pShape->smoothness), m_height(pShape->m_height), m_width(pShape->m_width), m_origin(pShape->m_origin), m_trianglepoints(pShape->m_trianglepoints),
		m_shape(pShape->m_shape), m_verticies(pShape->m_verticies), m_indicies(pShape->m_indicies)
	{
	}


	Vector2f Shape::GetPosition(float32 ratioX, float32 ratioY)
	{
		Vector2f position = m_Transform.GetPosition();

		if (m_shape == Shapes::Circle)
		{
			float32 size = m_radius * 2;

			position.x += size * ratioX;
			position.y += size * ratioY;

		}
		else if (m_shape == Shapes::Rectangle)
		{
			float32 sizeX = m_width * ratioX;
			float32 sizeY = m_height * ratioY;

			position.x += sizeX;
			position.y += sizeY;
		}

		return position;
	}

	void Shape::SetPosition(float32 x, float32 y, float32 ratioX, float32 ratioY)
	{
		float32 dx = 0.f;
		float32 dy = 0.f;
		if (m_shape == Shapes::Circle)
		{
			float32 size = m_radius * 2;

			x -= size * ratioX;
			y -= size * ratioY;

			dx = (x + m_radius) - m_center.x;
			dy = (y + m_radius) - m_center.y;

			m_center.x = x + m_radius;
			m_center.y = y + m_radius;

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

		m_Transform.SetPosition({ m_Transform.GetPosition().x + dx, m_Transform.GetPosition().y + dy });
	}

	void Shape::Move(Vector2f translation)
	{
		SetPosition(m_Transform.GetPosition().x + translation.x, m_Transform.GetPosition().y + translation.y, 0.0f, 0.0f);
	}
}