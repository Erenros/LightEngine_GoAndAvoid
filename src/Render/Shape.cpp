#include "Shape.h" 
#include <SDL.h>

namespace gcle
{
	Shape::~Shape()
	{
		for (auto* vertex : m_verticies)
		{
			if (vertex != nullptr)
			{
				delete vertex;
			}
		}
	}

	Shape::Shape(Shape* pShape) : m_texture(pShape->m_texture), m_Transform(pShape->m_Transform), m_radius(pShape->m_radius), m_center(pShape->m_center),
		m_smoothness(pShape->m_smoothness), m_height(pShape->m_height), m_width(pShape->m_width), m_origin(pShape->m_origin), m_trianglepoints(pShape->m_trianglepoints),
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

			dx = x - m_verticies[0]->position.x;
			dy = y - m_verticies[0]->position.y;
		}
		else
		{
			dx = x - m_verticies[0]->position.x;
			dy = y - m_verticies[0]->position.y;
		}

		for (auto& vertex : m_verticies) {
			vertex->position.x += dx;
			vertex->position.y += dy;
		}

		m_Transform.SetPosition({ m_Transform.GetPosition().x + dx, m_Transform.GetPosition().y + dy });
	}

	void Shape::Move(Vector2f translation)
	{
		SetPosition(m_Transform.GetPosition().x + translation.x, m_Transform.GetPosition().y + translation.y, 0.0f, 0.0f);
	}

	Rectangle::Rectangle(float32 x, float32 y, float32 height, float32 width, Color color) {
		{

			m_shape = Shapes::Rectangle;

			m_verticies.resize(4);
			m_indicies.resize(6);

			m_height = height;
			m_width = width;

			SDL_Color sdl_color{ color.r, color.g, color.b, color.a };

			m_verticies[0] = new SDL_Vertex{ {x, y}, sdl_color, {0.f, 0.f} };
			m_verticies[1] = new SDL_Vertex{ {x + m_width, y}, sdl_color, {1.f, 0.f} };
			m_verticies[2] = new SDL_Vertex{ {x, y + m_height}, sdl_color, {0.f, 1.f} };
			m_verticies[3] = new SDL_Vertex{ {x + m_width, y + m_height}, sdl_color, {1.f, 1.f} };

			m_indicies = {
				0, 1, 2,
				2, 3, 1
			};

			m_origin = { x, y };
			m_Transform.Initialize({ x, y }, 0);
		}
	}

	Triangle::Triangle(float32 x1, float32 y1, float32 x2, float32 y2, float32 x3, float32 y3, Color color){
		m_trianglepoints.push_back({ x1, y1 });
		m_trianglepoints.push_back({ x2, y2 });
		m_trianglepoints.push_back({ x2, y2 });
		m_trianglepoints.push_back({ x3, y3 });


		m_shape = Shapes::Triangle;

		m_verticies.resize(3);
		m_indicies.resize(3);

		SDL_Color sdl_color{ color.r, color.g, color.b, color.a };

		m_verticies[0] = new SDL_Vertex{ {x1, y1}, sdl_color, {0.f, 0.f} };
		m_verticies[1] = new SDL_Vertex{ {x2, y2}, sdl_color, {1.f, 0.f} };
		m_verticies[2] = new SDL_Vertex{ {x3, y3}, sdl_color, {1.f, 1.f} };

		m_indicies = { 0, 1, 2 };

		m_origin = { x1, y1 };

		m_Transform.Initialize({ x1, y1 }, 0);
	}

	Circle::Circle(float32 x, float32 y, float32 radius, int _smoothness, Color color){
		if (_smoothness < 3) {
			return;
		}

		m_smoothness = _smoothness;

		m_center.x = x + radius;
		m_center.y = y + radius;

		m_radius = radius;

		m_shape = Shapes::Circle;

		m_verticies.resize(_smoothness + 1);
		m_indicies.resize(_smoothness * 3);

		m_origin = { x, y };

		m_Transform.Initialize({ x, y }, 0);

		Degrees degreesBetweenPoints = 360.0f / _smoothness;

		SDL_Color sdl_color{ color.r, color.g, color.b, color.a };

		m_verticies[0] = new SDL_Vertex{ {x + radius, y + radius}, sdl_color, {1, 1} };

		for (int i = 0; i < _smoothness; i++) {
			Radians radian = MathGC::DegToRad(degreesBetweenPoints * i);

			float cx = x + sin(radian) * radius + radius;
			float cy = y + cos(radian) * radius + radius;

			m_verticies[i + 1] = new SDL_Vertex{ {cx, cy}, sdl_color, {1, 1} };

			m_indicies[i * 3] = 0;
			m_indicies[i * 3 + 1] = i + 1;
			m_indicies[i * 3 + 2] = (i + 1) % _smoothness + 1;
		}
	}
}