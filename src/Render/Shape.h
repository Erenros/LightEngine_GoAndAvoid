#pragma once
#include "SDL.h"
#include <vector>
#include <iostream> 
#include "MathGC.h"

enum class Shapes {
	Triangle,
	Rectangle,
	Circle
};

class Shape {

	SDL_Texture* m_texture = nullptr;

protected:


	//circle
	float32 m_radius = 0.0f;
	Vector2f m_center = { 0.0f, 0.0f };
	int smoothness = 16;

	//rectangle
	float32 m_height = 0.0f;
	float32 m_width = 0.0f;
	Vector2f m_coordinates = { 0.0f, 0.0f };

	Shapes m_shape = Shapes::Triangle;

	std::vector<SDL_Vertex> m_verticies;
	std::vector<int32> m_indicies;

	//Constructors 

	Shape() = default;  


public:
	//Getters 
	Shapes GetShape() { return m_shape; };
	SDL_Texture* GetTexture() { return m_texture; };
	std::vector<int32>& GetIndicies() { return m_indicies; };
	std::vector<SDL_Vertex>& GetVerticies() { return m_verticies; };

public:

	virtual float32 GetRadius() { return 0.f; };
	virtual float32 GetHeight() { return 0.f; };
	virtual float32 GetWidth() { return 0.f; };
	virtual int32 GetSmoothness() { return 0; };
	virtual Vector2f GetCenter() { return { 0, 0 }; };
	virtual Vector2f GetCoordinates() { return { 0, 0 }; };


public:
	//Setters 

	virtual void SetRadius(float32 radius) {};
	virtual void SetHeight(float32 height) {};
	virtual void SetWidth(float32 width) {};
	virtual void SetCenter(Vector2f center) {};
	virtual void SetCoordinates(Vector2f coordinates) {};
	
	void SetTexture(SDL_Texture* tex) {
		m_texture = tex;
	}

};


class Rectangle : public Shape { 

public:
	Rectangle(float32 x, float32 y, float32 height, float32 width, SDL_Color color) {
		m_shape = Shapes::Rectangle;

		m_verticies.resize(4);
		m_indicies.resize(6);

		m_height = height;
		m_width = width;

		m_verticies[0] = (SDL_Vertex{ {x, y}, color, {1, 1} });
		m_verticies[1] = (SDL_Vertex{ {x + m_width, y}, color, {1, 1} });
		m_verticies[2] = (SDL_Vertex{ {x, y + m_height}, color, {1, 1} });
		m_verticies[3] = (SDL_Vertex{ {x + m_width, y + m_height}, color, {1, 1} });

		m_indicies = {
			0, 1, 2,
			2, 3, 1
		};
	}


	//Getters

	float32 GetHeight() override {
		return m_height;
	}
	float32 GetWidth() override {
		return m_width;
	}
	Vector2f GetCoordinates() override {
		return m_coordinates;
	}


	//Setters

	void SetHeight(float32 height) override {
		m_height = height;
	}
	void SetWidth(float32 width) override {
		m_width = width;
	}
	void SetCoordinates(Vector2f coordinates) override {
		m_coordinates = coordinates;
	} 
};

class Triangle : public Shape { 
public:

	//Contructors

	Triangle(float32 x1, float32 y1, float32 x2, float32 y2, float32 x3, float32 y3, SDL_Color color) {
		m_shape = Shapes::Triangle;

		m_verticies.resize(3);
		m_indicies.resize(3);

		m_verticies[0] = SDL_Vertex{ {x1, y1}, color, {1, 1} };
		m_verticies[1] = SDL_Vertex{ {x2, y2}, color, {1, 1} };
		m_verticies[2] = SDL_Vertex{ {x3, y3}, color, {1, 1} }; 

		m_indicies = { 0, 1, 2 };
	} 
};

class Circle: public Shape { 
public:

	//Contructors

	Circle(int x, int y, int radius, int smoothness, SDL_Color color) {
		if (smoothness == 0) {
			return;
		}

		m_shape = Shapes::Circle;

		m_verticies.resize(smoothness + 1);
		m_indicies.resize(smoothness * 3);  


		Degrees degreesBetweenPoints = 360.0f / smoothness;

		m_verticies[0] = SDL_Vertex{ {(float)x, (float)y} , color, {1, 1} };

		for (int i = 0; i < smoothness; i++) {
			Radians radian = MathGC::DegToRad(degreesBetweenPoints * i);
			
			float cx = x + sin(radian) * radius;
			float cy = y + cos(radian) * radius;

			SDL_Vertex vertex{ {cx, cy}, color, {1, 1} };
			m_verticies[i + 1] = vertex;

			m_indicies[i * 3] = 0;
			m_indicies[i * 3 + 1] = i + 1;
			m_indicies[i * 3 + 2] = (i + 1) % smoothness + 1;
		}

	} 
};

