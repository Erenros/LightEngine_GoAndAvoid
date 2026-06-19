#pragma once
#include "SDL.h"
#include <vector>
#include <iostream> 
#include "MathGC.h"
#include "Engine/Transform.h"

namespace gcle
{
	enum class Shapes {
		Triangle,
		Rectangle,
		Circle,
		Count
	};

	class Shape {

		SDL_Texture* m_texture = nullptr;

	protected:
		Transform2D m_Transform;

		//circle
		float32 m_radius = 0.0f;
		Vector2f m_center = { 0.0f, 0.0f };
		int m_smoothness = 16;

		//rectangle
		float32 m_height = 0.0f;
		float32 m_width = 0.0f;
		Vector2f m_origin = { 0.0f, 0.0f };

		Shapes m_shape = Shapes::Triangle;

		std::vector<SDL_Vertex> m_verticies;
		std::vector<int32> m_indicies;

		//Constructors 

		Shape() = default;


	public:
		//Getters 
		Shapes GetShape()						{ return m_shape; };
		Vector2f GetOrigin()					{ return m_origin; }
		SDL_Texture* GetTexture()				{ return m_texture; };
		std::vector<int32>& GetIndicies()		{ return m_indicies; };
		std::vector<SDL_Vertex>& GetVerticies()	{ return m_verticies; };
		Vector2f GetPosition(float32 ratioX = 0.5f, float32 ratioY = 0.5f);

	public:

		virtual float32 GetWidth()		{ return 0.f; };
		virtual float32 GetHeight()		{ return 0.f; };
		virtual float32 GetRadius()		{ return 0.f; };
		virtual int32 GetSmoothness()	{ return 0; };
		virtual Vector2f GetCenter()	{ return { 0, 0 }; };


	public:
		//Setters 

		virtual void SetRadius(float32 radius) {};
		virtual void SetHeight(float32 height) {};
		virtual void SetWidth(float32 width) {};
		virtual void SetCenter(Vector2f center) {};

	public:

		void SetTexture(SDL_Texture* tex) { m_texture = tex; }
		void SetOrigin(Vector2f origin) { m_origin = origin; }
		void SetPosition(float32 x, float32 y, float32 ratioX = 0.5f, float32 ratioY = 0.5f);

	public:
		void Move(Vector2f translation);
	};


	class Rectangle : public Shape {

	public:
		Rectangle(float32 x, float32 y, float32 height, float32 width, SDL_Color color)
		{

			m_shape = Shapes::Rectangle;

			m_verticies.resize(4);
			m_indicies.resize(6);

			m_height = height;
			m_width = width;

			m_verticies[0] = (SDL_Vertex{ {x, y}, color, {0.f, 0.f} });
			m_verticies[1] = (SDL_Vertex{ {x + m_width, y}, color, {1.f, 0.f} });
			m_verticies[2] = (SDL_Vertex{ {x, y + m_height}, color, {0.f, 1.f} });
			m_verticies[3] = (SDL_Vertex{ {x + m_width, y + m_height}, color, {1.f, 1.f} });

			m_indicies = {
				0, 1, 2,
				2, 3, 1
			};

			m_origin = { x, y };
			m_Transform.Initialize({ x, y }, 0);
		}


		//Getters

		float32 GetHeight() override {
			return m_height;
		}
		float32 GetWidth() override {
			return m_width;
		}



		//Setters

		void SetHeight(float32 height) override {
			m_height = height;
		}
		void SetWidth(float32 width) override {
			m_width = width;
		}

	};

	class Triangle : public Shape {
	public:

		//Contructors

		Triangle(float32 x1, float32 y1, float32 x2, float32 y2, float32 x3, float32 y3, SDL_Color color) {
			m_shape = Shapes::Triangle;

			m_verticies.resize(3);
			m_indicies.resize(3);

			m_verticies[0] = SDL_Vertex{ {x1, y1}, color, {0.f, 0.f} };
			m_verticies[1] = SDL_Vertex{ {x2, y2}, color, {1, 0.f} };
			m_verticies[2] = SDL_Vertex{ {x3, y3}, color, {1.f, 1.f} };

			m_indicies = { 0, 1, 2 };

			m_origin = { x1, y1 };

			m_Transform.Initialize({ x1, y1 }, 0);
		}
	};

	class Circle : public Shape {
	public:

		//Contructors

		Circle(float32 x, float32 y, float32 radius, int _smoothness, SDL_Color color) {
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

			m_verticies[0] = SDL_Vertex{ {x + radius, y + radius} , color, {1, 1} };

			for (int i = 0; i < _smoothness; i++) {
				Radians radian = MathGC::DegToRad(degreesBetweenPoints * i);

				float cx = x + sin(radian) * radius + radius;
				float cy = y + cos(radian) * radius + radius;

				SDL_Vertex vertex{ {cx, cy}, color, {1, 1} };
				m_verticies[i + 1] = vertex;

				m_indicies[i * 3] = 0;
				m_indicies[i * 3 + 1] = i + 1;
				m_indicies[i * 3 + 2] = (i + 1) % _smoothness + 1;
			}
		}

		float32 GetRadius() override { return m_radius; };
		int32 GetSmoothness() override { return m_smoothness; };
		Vector2f GetCenter() override { return m_center; };
		
		
		
	};
}

