#pragma once
#include <vector>
#include <iostream> 
#include "MathGC.h"
#include "Engine/Transform.h"
#include "Texture.h"

struct SDL_Vertex;

struct Color {
	uint8 r;
	uint8 g;
	uint8 b;
	uint8 a;
};

namespace gcle
{
	enum class Shapes {
		Triangle,
		Rectangle,
		Circle,
		Count
	};

	class Shape {

		Texture* m_texture = nullptr;

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

		//triangle
		std::vector<Vector2f> m_trianglepoints;

		Shapes m_shape = Shapes::Triangle;

		std::vector<SDL_Vertex*> m_verticies;
		std::vector<int32> m_indicies;

		//Constructors 

		Shape() = default;
		virtual ~Shape();
		Shape(Shape* pShape);


	public:
		//Getters 
		Shapes GetShape() { return m_shape; };
		Vector2f GetOrigin() { return m_origin; }
		Texture* GetTexture() { return m_texture; };
		std::vector<int32>& GetIndicies() { return m_indicies; };
		std::vector<SDL_Vertex*>& GetVerticies() { return m_verticies; };
		Vector2f GetPosition(float32 ratioX = 0.5f, float32 ratioY = 0.5f);

	public:

		virtual float32 GetWidth() { return 0.f; };
		virtual float32 GetHeight() { return 0.f; };
		virtual float32 GetRadius() { return 0.f; };
		virtual int32 GetSmoothness() { return 0; };
		virtual Vector2f GetCenter() { return { 0, 0 }; };
		virtual std::vector<Vector2f> GetTrianglePoints() { return m_trianglepoints; };


	public:
		//Setters 

		virtual void SetRadius(float32 radius) {};
		virtual void SetHeight(float32 height) {};
		virtual void SetWidth(float32 width) {};
		virtual void SetCenter(Vector2f center) {};
		virtual void SetTrianglePoints(std::vector<Vector2f> newTrianglePoints) {};

	public:

		void SetTexture(Texture* tex) { m_texture = tex; }
		void SetOrigin(Vector2f origin) { m_origin = origin; }
		void SetPosition(float32 x, float32 y, float32 ratioX = 0.5f, float32 ratioY = 0.5f);

	public:
		void Move(Vector2f translation);
	};


	class Rectangle : public Shape {

	public:
		Rectangle(float32 x, float32 y, float32 height, float32 width, Color color);
	


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
		void SetWidth(float32 width) override
		{
			m_width = width;
		}

	};

	class Triangle : public Shape {
	public:

		//Contructors

		Triangle(float32 x1, float32 y1, float32 x2, float32 y2, float32 x3, float32 y3, Color color);
	};

	class Circle : public Shape {
	public:

		//Contructors

		Circle(float32 x, float32 y, float32 radius, int _smoothness, Color color);


		float32 GetRadius() override { return m_radius; };
		int32 GetSmoothness() override { return m_smoothness; };
		Vector2f GetCenter() override { return m_center; };

	};
}