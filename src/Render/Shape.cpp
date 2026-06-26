#include "Shape.h" 
#include <SDL.h>
#include "Engine/Entity.h"

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

	Shape::Shape(Entity* owner) : mp_Owner(owner)
	{
	}

	Shape::Shape(Shape* pShape) : mp_texture(pShape->mp_texture), m_Transform(pShape->m_Transform), m_radius(pShape->m_radius), m_center(pShape->m_center),
		m_smoothness(pShape->m_smoothness), m_height(pShape->m_height), m_width(pShape->m_width), m_origin(pShape->m_origin), m_trianglepoints(pShape->m_trianglepoints),
		m_shape(pShape->m_shape), m_localPositions(pShape->m_localPositions), m_indicies(pShape->m_indicies)
	{
		m_verticies.resize(pShape->m_verticies.size());
		for (size_t i = 0; i < m_verticies.size(); i++)
		{
			m_verticies[i] = new SDL_Vertex(*pShape->m_verticies[i]);
		}
		m_Transform.SetDirty();
	}


	Vector2f Shape::GetPosition(float32 ratioX, float32 ratioY)
	{
		Vector2f pivot = m_Transform.GetPosition();

		if (m_shape == Shapes::Circle)
		{
			float32 size = GetRadius() * 2.f;
			Vector2f topLeft = { pivot.x - size * 0.5f, pivot.y - size * 0.5f };
			return { topLeft.x + size * ratioX, topLeft.y + size * ratioY };
		}
		else if (m_shape == Shapes::Rectangle)
		{
			float32 sizeX = GetWidth();
			float32 sizeY = GetHeight();
			Vector2f topLeft = { pivot.x - sizeX * 0.5f, pivot.y - sizeY * 0.5f };
			return { topLeft.x + sizeX * ratioX, topLeft.y + sizeY * ratioY };
		}

		return pivot;
	}

	void Shape::SetPosition(float32 x, float32 y, float32 ratioX, float32 ratioY)
	{
		Vector2f newPivot = { x, y };

		if (m_shape == Shapes::Circle)
		{
			float32 size = GetRadius() * 2.f;
			newPivot.x = x - size * ratioX + size * 0.5f;
			newPivot.y = y - size * ratioY + size * 0.5f;
		}
		else if (m_shape == Shapes::Rectangle)
		{
			float32 sizeX = GetWidth();
			float32 sizeY = GetHeight();
			newPivot.x = x - sizeX * ratioX + sizeX * 0.5f;
			newPivot.y = y - sizeY * ratioY + sizeY * 0.5f;
		}

		m_Transform.SetPosition(newPivot);
		m_center = newPivot;
	}

	void Shape::Move(Vector2f translation)
	{
		Vector2f pivot = m_Transform.GetPosition();
		m_Transform.SetPosition({ pivot.x + translation.x, pivot.y + translation.y });
		m_center.x += translation.x;
		m_center.y += translation.y;
	}

	void Shape::SetScale(Vector2f scale)
	{
		m_Transform.SetScale(scale);
	}

	void Shape::ScaleBy(Vector2f factor)
	{
		Vector2f current = m_Transform.GetScale();
		m_Transform.SetScale({ current.x * factor.x, current.y * factor.y });
	}

	void Shape::SetRotation(Degrees angle)
	{
		m_Transform.SetDegAngle(angle);
		m_Transform.SetRadAngle(MathGC::DegToRad(angle));
	}

	void Shape::Rotate(Degrees delta)
	{
		SetRotation(m_Transform.GetDegAngle() + delta);
	}

	void Shape::UpdateRenderVertices()
	{
		if (!m_Transform.IsDirty())
			return;

		Matrix3x3 mat = m_Transform.GetMatrix();

		for (size_t i = 0; i < m_localPositions.size() && i < m_verticies.size(); i++)
		{
			Vector2f world = mat.TransformPoint(m_localPositions[i]);
			m_verticies[i]->position.x = world.x;
			m_verticies[i]->position.y = world.y;
		}

		m_Transform.ClearDirty();
	}

	std::vector<SDL_Vertex*>& Shape::GetVerticies()
	{
		UpdateRenderVertices();
		return m_verticies;
	}

	Rectangle::Rectangle(float32 x, float32 y, float32 height, float32 width, Color color, Entity* owner) : Shape(owner)
	{
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

			Vector2f pivot = { x + m_width * 0.5f, y + m_height * 0.5f };
			m_Transform.Initialize(pivot, 0);

			m_localPositions = {
				{ -m_width * 0.5f, -m_height * 0.5f },
				{  m_width * 0.5f, -m_height * 0.5f },
				{ -m_width * 0.5f,  m_height * 0.5f },
				{  m_width * 0.5f,  m_height * 0.5f }
			};

			m_Transform.SetDirty();
		}
	}

	void Rectangle::SetHeight(float32 height)
	{
		m_height = height;
		m_localPositions = {
			{ -m_width * 0.5f, -m_height * 0.5f },
			{  m_width * 0.5f, -m_height * 0.5f },
			{ -m_width * 0.5f,  m_height * 0.5f },
			{  m_width * 0.5f,  m_height * 0.5f }
		};
		m_Transform.SetDirty();
	}

	void Rectangle::SetWidth(float32 width)
	{
		m_width = width;
		m_localPositions = {
			{ -m_width * 0.5f, -m_height * 0.5f },
			{  m_width * 0.5f, -m_height * 0.5f },
			{ -m_width * 0.5f,  m_height * 0.5f },
			{  m_width * 0.5f,  m_height * 0.5f }
		};
		m_Transform.SetDirty();
	}

	Triangle::Triangle(float32 x1, float32 y1, float32 x2, float32 y2, float32 x3, float32 y3, Color color, Entity* owner) : Shape(owner)
	{
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

		float32 minX = std::min({ x1, x2, x3 });
		float32 maxX = std::max({ x1, x2, x3 });
		float32 minY = std::min({ y1, y2, y3 });
		float32 maxY = std::max({ y1, y2, y3 });
		Vector2f pivot = { (minX + maxX) * 0.5f, (minY + maxY) * 0.5f };

		m_Transform.Initialize(pivot, 0);
		m_center = pivot;

		m_localPositions = {
			{ x1 - pivot.x, y1 - pivot.y },
			{ x2 - pivot.x, y2 - pivot.y },
			{ x3 - pivot.x, y3 - pivot.y }
		};

		m_Transform.SetDirty();
	}

	void Triangle::SetTrianglePoints(std::vector<Vector2f> newTrianglePoints)
	{
		if (newTrianglePoints.size() < 3)
			return;

		m_trianglepoints = newTrianglePoints;

		float32 minX = std::min({ newTrianglePoints[0].x, newTrianglePoints[1].x, newTrianglePoints[2].x });
		float32 maxX = std::max({ newTrianglePoints[0].x, newTrianglePoints[1].x, newTrianglePoints[2].x });
		float32 minY = std::min({ newTrianglePoints[0].y, newTrianglePoints[1].y, newTrianglePoints[2].y });
		float32 maxY = std::max({ newTrianglePoints[0].y, newTrianglePoints[1].y, newTrianglePoints[2].y });
		Vector2f pivot = { (minX + maxX) * 0.5f, (minY + maxY) * 0.5f };

		m_Transform.SetPosition(pivot);
		m_center = pivot;

		m_localPositions.clear();
		for (int i = 0; i < 3; i++)
		{
			m_localPositions.push_back({ newTrianglePoints[i].x - pivot.x, newTrianglePoints[i].y - pivot.y });
		}

		m_Transform.SetDirty();
	}

	Circle::Circle(float32 x, float32 y, float32 radius, int _smoothness, Color color, Entity* owner) : Shape(owner)
	{
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

		m_Transform.Initialize(m_center, 0);

		Degrees degreesBetweenPoints = 360.0f / _smoothness;

		SDL_Color sdl_color{ color.r, color.g, color.b, color.a };

		m_verticies[0] = new SDL_Vertex{ {x + radius, y + radius}, sdl_color, {1, 1} };
		m_localPositions.push_back({ 0.f, 0.f });

		for (int i = 0; i < _smoothness; i++) {
			Radians radian = MathGC::DegToRad(degreesBetweenPoints * i);

			float cx = sin(radian) * radius;
			float cy = cos(radian) * radius;

			m_verticies[i + 1] = new SDL_Vertex{ {x + radius + cx, y + radius + cy}, sdl_color, {1, 1} };
			m_localPositions.push_back({ cx, cy });

			m_indicies[i * 3] = 0;
			m_indicies[i * 3 + 1] = i + 1;
			m_indicies[i * 3 + 2] = (i + 1) % _smoothness + 1;
		}

		m_Transform.SetDirty();
	}

	void Circle::SetRadius(float32 radius)
	{
		m_radius = radius;

		Degrees degreesBetweenPoints = 360.0f / m_smoothness;

		for (int i = 0; i < m_smoothness; i++)
		{
			Radians rad = MathGC::DegToRad(degreesBetweenPoints * i);
			m_localPositions[i + 1] = { sin(rad) * radius, cos(rad) * radius };
		}

		m_Transform.SetDirty();
	}

	void Shape::SetTextureRect(int16 x, int16 y, int16 w, int16 h, int16 textW, int16 textH)
	{
		if (m_shape == Shapes::Rectangle)
		{
			float32 u0 = x / static_cast<float32>(textW);
			float32 v0 = y / static_cast<float32>(textH);

			float32 u1 = (x + w) / static_cast<float32>(textW);
			float32 v1 = (y + h) / static_cast<float32>(textH);

			m_verticies[0]->tex_coord = { u0, v0 };
			m_verticies[1]->tex_coord = { u1, v0 };
			m_verticies[3]->tex_coord = { u1, v1 };
			m_verticies[2]->tex_coord = { u0, v1 };
		}
		else if (m_shape == Shapes::Triangle)
		{
			float32 u0 = x / static_cast<float32>(textW);
			float32 v0 = y / static_cast<float32>(textH);

			float32 u1 = (x + w) / static_cast<float32>(textW);
			float32 v1 = (y + h) / static_cast<float32>(textH);

			m_verticies[0]->tex_coord = { u0, v0 };
			m_verticies[1]->tex_coord = { u1, v0 };
			m_verticies[2]->tex_coord = { u1, v1 };
		}
		else if (m_shape == Shapes::Circle)
		{
			//NON LES CERCLES C'EST PAS MARRANTS
		}
	}
}