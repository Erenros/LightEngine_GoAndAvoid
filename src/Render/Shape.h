#pragma once
#include <vector>
#include <iostream> 
#include "MathGC.h"
#include "Engine/Transform.h"
#include "Texture.h"
#include "Engine/RessourceManager.h"
#include "Sprite.h"

class Entity;
struct SDL_Vertex;
struct SDL_FPoint;

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

		TextureStruct* mp_texture = nullptr;

	protected:
		Transform2D m_Transform;
		Entity* mp_Owner;

		//circle
		float32 m_radius = 0.0f;
		Vector2f m_center = { 0.0f, 0.0f };
		int32 m_smoothness = 16;

		//rectangle
		float32 m_height = 0.0f;
		float32 m_width = 0.0f;
		Vector2f m_origin = { 0.0f, 0.0f };

		//triangle
		std::vector<Vector2f> m_trianglepoints;

		Shapes m_shape = Shapes::Triangle;
		 
		std::vector<Vector2f> m_localPositions;

		std::vector<SDL_Vertex*> m_verticies;
		std::vector<int32> m_indicies;

		std::vector<SDL_FPoint*> m_hollowPoints;
		std::vector<SDL_FPoint*> m_debugContour;
		 
		void UpdateRenderVertices();

		//Constructors 

		Shape(Entity* owner);
		Shape(const Shape& pShape);
		Shape& operator=(const Shape&) = delete;

	public:
		virtual ~Shape();

	public:

		//Getters 
		Shapes GetShape() { return m_shape; };
		Vector2f GetOrigin() { return m_origin; }
		Sprite* GetTexture() { return (mp_texture == nullptr ? nullptr : mp_texture->mp_texture);};
		std::vector<int32>& GetIndicies() { return m_indicies; }; 
		std::vector<SDL_Vertex*>& GetVerticies();
		Vector2f GetPosition(float32 ratioX = 0.5f, float32 ratioY = 0.5f);

		Vector2f GetScale() { return m_Transform.GetScale(); }
		Degrees GetRotation() { return m_Transform.GetDegAngle(); }

		Transform2D* GetTransform() { return &m_Transform; }


	public:

		virtual float32 GetWidth() { return 0.f; };
		virtual float32 GetHeight() { return 0.f; };
		virtual float32 GetRadius() { return 0.f; };
		virtual int32 GetSmoothness() { return 0; };
		virtual Entity* GetOwner() { return mp_Owner; };
		virtual Vector2f GetCenter() { return { 0, 0 }; };
		virtual std::vector<SDL_FPoint*>& GetHollow();
		virtual std::vector<Vector2f> GetTrianglePoints() { return m_trianglepoints; };


	public:
		//Setters 

		virtual void SetRadius(float32 radius) {};
		virtual void SetHeight(float32 height) {};
		virtual void SetWidth(float32 width) {};
		virtual void SetCenter(Vector2f center) {};
		virtual void SetTrianglePoints(std::vector<Vector2f> newTrianglePoints) {};

	public:

		void SetTexture(TextureStruct* tex) { mp_texture = tex; }
		void SetOrigin(Vector2f origin) { m_origin = origin; }
		void SetPosition(float32 x, float32 y, float32 ratioX = 0.5f, float32 ratioY = 0.5f);
		 
		void SetScale(Vector2f scale);
		void SetScale(float32 scale) { SetScale({ scale, scale }); } 
		void ScaleBy(Vector2f factor);
		 
		void SetRotation(Degrees angle); 
		void Rotate(Degrees delta);

	public:
		void Move(Vector2f translation);

		virtual void SetTextureRect(int16 x, int16 y, int16 w, int16 h, int16 textW, int16 textH);

		virtual Shape* Clone() const = 0;
	};


	class Rectangle : public Shape {

	public:
		Rectangle(float32 x, float32 y, float32 height, float32 width, Color color, Entity* owner);
	
		Shape* Clone() const override { return GCLE_NEW Rectangle(*this); }

		//Getters
		 
		float32 GetHeight() override {
			return m_height * m_Transform.GetScale().y;
		}
		float32 GetWidth() override {
			return m_width * m_Transform.GetScale().x;
		}

		std::vector<SDL_FPoint*>& GetHollow() override;


		//Setters

		void SetHeight(float32 height) override;
		void SetWidth(float32 width) override;


		//void SetTextureRect(int16 x, int16 y, int16 w, int16 h, int16 textW, int16 textH) override;
	};

	class Triangle : public Shape {
	public:

		//Contructors

		Triangle(float32 x1, float32 y1, float32 x2, float32 y2, float32 x3, float32 y3, Color color, Entity* owner);

		Shape* Clone() const override { return GCLE_NEW Triangle(*this); }

		//void SetTextureRect(int16 x, int16 y, int16 w, int16 h, int16 textW, int16 textH) override;
		void SetTrianglePoints(std::vector<Vector2f> newTrianglePoints) override;

		std::vector<SDL_FPoint*>& GetHollow() override;
	};

	class Circle : public Shape {
	public:

		//Contructors

		Circle(float32 x, float32 y, float32 radius, int _smoothness, Color color, Entity* owner);

		Shape* Clone() const override { return GCLE_NEW Circle(*this); }

		float32 GetRadius() override { return m_radius * m_Transform.GetScale().x; };
		int32 GetSmoothness() override { return m_smoothness; };
		Vector2f GetCenter() override { return m_center; };

		std::vector<SDL_FPoint*>& GetHollow() override;

		//void SetTextureRect(int16 x, int16 y, int16 w, int16 h, int16 textW, int16 textH) override;
		void SetRadius(float32 radius) override;

	};
}