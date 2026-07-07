#include "Shape.h" 
#include <SDL3/SDL.h>
#include "Engine/Entity.h"

const Color Color::White = { 255,255,255,255 };
const Color Color::Black = { 0,0,0,255 };
const Color Color::Red = { 255,0,0,255 };
const Color Color::Green = { 0,255,0,255 };
const Color Color::Blue = { 0,0,255,255 };
const Color Color::Yellow = { 255,255,0,255 };
const Color Color::Cyan = { 0,255,255,255 };
const Color Color::Magenta = { 255,0,255,255 };
const Color Color::Transparent = { 0,0,0,0 };

namespace gcle
{
	SDL_FColor ToSDLColor(Color c)
	{
		return SDL_FColor{ c.r / 255.0f, c.g / 255.0f, c.b / 255.0f, c.a / 255.0f };
	}

	Shape::~Shape()
	{
		for (SDL_Vertex* vertex : m_verticies)
		{
			delete vertex;
		}
		m_verticies.clear();
		 
		for (SDL_FPoint* point : m_hollowPoints)
		{
			delete point;
		}
		m_hollowPoints.clear();

		m_debugContour.clear();
		m_localPositions.clear();
		m_indicies.clear();
		m_trianglepoints.clear();
	}

	Shape::Shape(Entity* owner) : mp_Owner(owner)
	{
	}

	Shape::Shape(const Shape& pShape) : mp_texture(pShape.mp_texture), m_Transform(pShape.m_Transform), m_radius(pShape.m_radius), m_center(pShape.m_center),
		m_smoothness(pShape.m_smoothness), m_height(pShape.m_height), m_width(pShape.m_width), m_origin(pShape.m_origin), m_trianglepoints(pShape.m_trianglepoints),
		m_shape(pShape.m_shape), m_localPositions(pShape.m_localPositions), m_indicies(pShape.m_indicies), mp_Owner(pShape.mp_Owner)
	{

		m_verticies.resize(pShape.m_verticies.size());
		for (uint64 i = 0; i < pShape.m_verticies.size(); ++i)
		{
			if (pShape.m_verticies[i])
				m_verticies[i] = GCLE_NEW SDL_Vertex(*pShape.m_verticies[i]);
			else
				m_verticies[i] = nullptr;
		}

		m_hollowPoints.resize(pShape.m_hollowPoints.size());
		for (uint64 i = 0; i < pShape.m_hollowPoints.size(); ++i)
		{
			if (pShape.m_hollowPoints[i])
				m_hollowPoints[i] = GCLE_NEW SDL_FPoint(*pShape.m_hollowPoints[i]);
			else
				m_hollowPoints[i] = nullptr;
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

	Color Shape::GetColor() const
	{
		if (m_verticies.empty() || m_verticies[0] == nullptr)
			return { 255, 255, 255, 255 };

		const SDL_FColor& c = m_verticies[0]->color;

		return
		{
			static_cast<uint8>(c.r * 255.f),
			static_cast<uint8>(c.g * 255.f),
			static_cast<uint8>(c.b * 255.f),
			static_cast<uint8>(c.a * 255.f)
		};
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
	}

	void Shape::Rotate(Degrees delta)
	{
		SetRotation(m_Transform.GetDegAngle() + delta);
	}

	void Shape::SetColor(Color color)
	{
		SDL_FColor sdlColor = ToSDLColor(color);

		for (SDL_Vertex* vertex : m_verticies)
		{
			if (vertex != nullptr)
			{
				vertex->color = sdlColor;
			}
		}
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

			m_hollowPoints[i]->x = world.x;
			m_hollowPoints[i]->y = world.y;
		}

		m_Transform.ClearDirty();
	}

	std::vector<SDL_FPoint*>& Shape::GetHollow()
	{
		UpdateRenderVertices();
		return m_hollowPoints;
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
			m_hollowPoints.resize(4);

			m_indicies.resize(6);

			m_height = height;
			m_width = width;

			SDL_FColor sdl_color = ToSDLColor(color);

			m_verticies[0] = GCLE_NEW SDL_Vertex{ {x, y}, sdl_color, {0.f, 0.f} };
			m_verticies[1] = GCLE_NEW SDL_Vertex{ {x + m_width, y}, sdl_color, {1.f, 0.f} };
			m_verticies[2] = GCLE_NEW SDL_Vertex{ {x, y + m_height}, sdl_color, {0.f, 1.f} };
			m_verticies[3] = GCLE_NEW SDL_Vertex{ {x + m_width, y + m_height}, sdl_color, {1.f, 1.f} };

			for (int32 i = 0; i < static_cast<int32>(m_verticies.size()); i++)
			{
				m_hollowPoints[i] = GCLE_NEW SDL_FPoint{ m_verticies[i]->position.x, m_verticies[i]->position.y };
			}

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

	std::vector<SDL_FPoint*>& Rectangle::GetHollow()
	{
		UpdateRenderVertices();

		if (m_debugContour.size() != 5)
			m_debugContour.resize(5);

		m_debugContour[0] = m_hollowPoints[0];
		m_debugContour[1] = m_hollowPoints[1];
		m_debugContour[2] = m_hollowPoints[3];
		m_debugContour[3] = m_hollowPoints[2];
		m_debugContour[4] = m_hollowPoints[0];

		return m_debugContour;
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
		m_trianglepoints.push_back({ x3, y3 });


		m_shape = Shapes::Triangle;

		m_verticies.resize(3);
		m_hollowPoints.resize(3);
		m_indicies.resize(3);

		SDL_FColor sdl_color = ToSDLColor(color);

		m_verticies[0] = GCLE_NEW SDL_Vertex{ {x1, y1}, sdl_color, {0.f, 0.f} };
		m_verticies[1] = GCLE_NEW SDL_Vertex{ {x2, y2}, sdl_color, {1.f, 0.f} };
		m_verticies[2] = GCLE_NEW SDL_Vertex{ {x3, y3}, sdl_color, {1.f, 1.f} };


		for (int i = 0; i < 3; ++i)
		{
			m_hollowPoints[i] = GCLE_NEW SDL_FPoint{};
		}

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

	std::vector<SDL_FPoint*>& Triangle::GetHollow()
	{
		UpdateRenderVertices();

		if (m_debugContour.size() != 4)
			m_debugContour.resize(4);

		m_debugContour[0] = m_hollowPoints[0];
		m_debugContour[1] = m_hollowPoints[1];
		m_debugContour[2] = m_hollowPoints[2];
		m_debugContour[3] = m_hollowPoints[0]; 

		return m_debugContour;
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
		m_hollowPoints.resize(_smoothness + 1);
		m_indicies.resize(_smoothness * 3);

		m_origin = { x, y };

		m_Transform.Initialize(m_center, 0);

		Degrees degreesBetweenPoints = 360.0f / _smoothness;

		SDL_FColor sdl_color = ToSDLColor(color);

		m_verticies[0] = new SDL_Vertex{ {x + radius, y + radius}, sdl_color, {0.5f, 0.5f} };
		m_localPositions.push_back({ 0.f, 0.f });

		for (int i = 0; i < _smoothness; i++)
		{
			Radians radian = MathGC::DegToRad(degreesBetweenPoints * i);

			float cx = sin(radian) * radius;
			float cy = cos(radian) * radius;

			float u = 0.5f + (cx / radius) * 0.5f;
			float v = 0.5f + (cy / radius) * 0.5f;

			m_verticies[i + 1] = new SDL_Vertex{
				{x + radius + cx, y + radius + cy},
				sdl_color,
				{u, v}
			};

			m_localPositions.push_back({ cx, cy });

			m_indicies[i * 3] = 0;
			m_indicies[i * 3 + 1] = i + 1;
			m_indicies[i * 3 + 2] = (i + 1) % _smoothness + 1;
		}


		for (int i = 0; i < _smoothness + 1; ++i)
		{
			m_hollowPoints[i] = GCLE_NEW SDL_FPoint{};
		}

		m_Transform.SetDirty();
	}

	std::vector<SDL_FPoint*>& Circle::GetHollow()
	{
		UpdateRenderVertices();

		int32 count = m_smoothness + 1; 

		if (static_cast<int32>(m_debugContour.size()) != count)
			m_debugContour.resize(count);

		for (int32 i = 0; i < m_smoothness; i++)
			m_debugContour[i] = m_hollowPoints[i + 1]; 

		m_debugContour[m_smoothness] = m_hollowPoints[1]; 

		return m_debugContour;
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
			float32 u0 = x / static_cast<float32>(textW);
			float32 v0 = y / static_cast<float32>(textH);
			float32 u1 = (x + w) / static_cast<float32>(textW);
			float32 v1 = (y + h) / static_cast<float32>(textH);
			m_verticies[0]->tex_coord = { (u0 + u1) * 0.5f, (v0 + v1) * 0.5f };
			for (int i = 1; i < m_smoothness + 1; i++)
			{
				m_verticies[i]->tex_coord = { u0 + (u1 - u0) * 0.5f * (1.f + m_localPositions[i].x / m_radius),
											  v0 + (v1 - v0) * 0.5f * (1.f + m_localPositions[i].y / m_radius) };
			}
		}
	}
}