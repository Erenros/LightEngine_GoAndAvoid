#pragma once
#include "SDL.h"
#include <vector>
#include <iostream>
#include "Engine/Window.h"
#include "Core/Math.hpp"

enum class Shapes {
	Triangle,
	Rectangle,
	Circle
};

class Shape {

	SDL_Texture* m_texture = nullptr;

protected:


	//circle
	int m_radius;
	Vector2<int> m_center;
	int smoothness;

	//rectangle
	int m_height;
	int m_width;
	Vector2<int> m_coordinates;

	Shapes m_shapes;

	int m_vertexNbr;
	int m_indicesNbr;

	//Constructors 

	Shape() = default;
	Shape(int vertexNbr, int indicesNbr) :m_vertexNbr(vertexNbr), m_indicesNbr(indicesNbr) {};

	virtual void Draw(Window* window) = 0;


public:
	//Getters 

	virtual int GetRadius() { return 0.f; };
	virtual int GetHeight() { return 0.f; };
	virtual int GetWidth() { return 0.f; };
	virtual int GetSmoothness() { return 0.f; };
	virtual Vector2<int> GetCenter() { return { 0, 0 }; };
	virtual Vector2<int> GetCoordinates() { return { 0, 0 }; };

	SDL_Texture* GetTexture() { return m_texture; };

	//Setters 

	virtual void SetRadius(int radius) {};
	virtual void SetHeight(int height) {};
	virtual void SetWidth(int width) {};
	virtual void SetCenter(Vector2<int> center) {};
	virtual void SetCoordinates(Vector2<int> coordinates) {};
	
	void SetTexture(SDL_Texture* tex) {
		m_texture = tex;
	}

};


class Rectangle : public Shape {

	SDL_Vertex m_vertices[4];

	//Constructors

	const int m_rectangleIndices[6] = {
		0,1,2,
		2,3,1
	};

public:
	Rectangle(int x, int y, float height, float width, SDL_Color color) {
		m_shapes = Shapes::Rectangle;

		m_height = height;
		m_width = width;

		SDL_Vertex vertex1 = { {x, y}, color, {1, 1} };
		SDL_Vertex vertex2 = { {x + m_width, y}, color, {1, 1} };
		SDL_Vertex vertex3 = { {x, y + m_height}, color, {1, 1} };
		SDL_Vertex vertex4 = { {x + m_width, y + m_height}, color, {1, 1} };

		m_vertices[0] = vertex1;
		m_vertices[1] = vertex2;
		m_vertices[2] = vertex3;
		m_vertices[3] = vertex4;

		m_indicesNbr = 6;
		m_vertexNbr = 4;

	}


	//Getters

	int GetHeight() override {
		return m_height;
	}
	int GetWidth() override {
		return m_width;
	}
	Vector2<int> GetCoordinates() override {
		return m_coordinates;
	}


	//Setters

	void SetHeight(int height) override {
		m_height = height;
	}
	void SetWidth(int width) override {
		m_width = width;
	}
	void SetCoordinates(Vector2<int> coordinates) override {
		m_coordinates = coordinates;
	}



	void Draw(Window* window);

};

class Triangle : public Shape {

	SDL_Vertex m_vertices[3];

public:

	//Contructors

	Triangle(int x1, int y1, int x2, int y2, int x3, int y3, SDL_Color color) {
		m_shapes = Shapes::Triangle;


		m_indicesNbr = 0;
		m_vertexNbr = 3;

		SDL_Vertex vertex1 = { {x1, y1}, color, {1, 1} };
		SDL_Vertex vertex2 = { {x2, y2}, color, {1, 1} };
		SDL_Vertex vertex3 = { {x3, y3}, color, {1, 1} };

		m_vertices[0] = vertex1;
		m_vertices[1] = vertex2;
		m_vertices[2] = vertex3;
	}

	void Draw(Window* pWindow) override;
};

class Circle: public Shape {
	SDL_Vertex* m_vertices;

public:

	//Contructors

	Circle(int x, int y, int radius, int smoothness, SDL_Color color) {
		m_shapes = Shapes::Circle;

		m_vertices = static_cast<SDL_Vertex*>(malloc(sizeof(SDL_Vertex) * smoothness));

		m_indicesNbr = 0;
		m_vertexNbr = smoothness;

		Math::Degrees degreesBetweenPoints = 360 / smoothness;

		for (int i = 0; i < smoothness; i++) {
			Math::Radians radian = Math::DegToRad(degreesBetweenPoints * i);
			
			x = sin(radian);
			y = cos(radian);

			SDL_Vertex vertex{ {x, y}, color, {1, 1} };
			m_vertices[i] = vertex;
		}

	}

	~Circle() { delete m_vertices; }

	void Draw(Window* pWindow) override;
};

